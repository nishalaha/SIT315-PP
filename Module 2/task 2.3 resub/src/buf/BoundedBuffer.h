#ifndef BOUNDED_BUFFER_H
#define BOUNDED_BUFFER_H

#include <ostream>
#include <pthread.h>
#include <semaphore.h>

/**
 * A bounded buffer of the specified type.
 */
template<typename T>
struct BoundedBuffer
{
	/** Array that stores elements of the buffer. */
	T* arr;

	/** Semaphore that tracks the number of elements in the buffer. */
	sem_t semHasElements;

	/** Semaphore that tracks the availbility of space in the buffer. */
	sem_t semHasSpace;

	/** Mutex that serializes access to the buffer. */
	pthread_mutex_t mutex;

	/** Index to which the next call to `produce` should assignment an element. */
	unsigned int idxProduce = 0;

	/** Index from which the next call to `consume` should read the element. */
	unsigned int idxConsume = 0;

	/** The capacity of `BoundedBuffer`. */
	const unsigned int capacity;

	/**
	 * Creates a `BoundedBuffer` of the specified capacity.
	 */
	BoundedBuffer(unsigned int _capacity): capacity(_capacity)
	{
		// Allocate memory to store elements.
		arr = (T*)malloc(sizeof(T) * _capacity);

		// Initialize mutexes and semaphores.
		pthread_mutex_init(&mutex, nullptr);
		sem_init(&semHasElements, false, 0);
		sem_init(&semHasSpace, false, _capacity);
	}

	/**
	 * Releases resources acquired by the `BoundedBuffer`.
	 */
	~BoundedBuffer()
	{
		// Release mutexes and semaphores.
		pthread_mutex_destroy(&mutex);
		sem_destroy(&semHasElements);
		sem_destroy(&semHasSpace);

		// Release allocated memory to store elements.
		free(arr);
	}

	/**
	 * Adds the specified element to the `BoundedBuffer`.
	 */
	void produce(T element)
	{
		sem_wait(&semHasSpace);
		pthread_mutex_lock(&mutex);

		arr[idxProduce] = element;
		idxProduce = (idxProduce + 1) % capacity;

		pthread_mutex_unlock(&mutex);
		sem_post(&semHasElements);
	}

	/**
	 * Retrieves the next element from the `BoundedBuffer`.
	 */
	T consume()
	{
		sem_wait(&semHasElements);
		pthread_mutex_lock(&mutex);

		T value = arr[idxConsume];
		idxConsume = (idxConsume + 1) % capacity;

		pthread_mutex_unlock(&mutex);
		sem_post(&semHasSpace);

		return value;
	}

	/**
	 * Retrieves the number of elements that the `BoundedBuffer` currently contains.
	 * Note that this method _is not threadsafe_.
	 */
	unsigned int count()
	{
		int c;
		sem_getvalue(&semHasElements, &c);
		return c;
	}

	/**
	 * Outputs the `BoundedBuffer` into the specified stream, in TSV format.
	 */
	friend std::ostream& operator<<(std::ostream& stream, BoundedBuffer& buf)
	{
		pthread_mutex_lock(&buf.mutex);

		unsigned int c = buf.count();
		unsigned int arrIdx = buf.idxConsume;

		for (unsigned int i = 0; i < c; i++)
		{
			stream << buf.arr[arrIdx++ % buf.capacity];

			if (i != c - 1)
				stream << '\t';
		}

		pthread_mutex_unlock(&buf.mutex);
		return stream;
	}
};

#endif
