#pragma once


/**
 * Interface for clonable objects.
 */
struct Clonable {
	/**
	 * Clones the object.
	 *
	 * \return A new object of the same type with the same data. The returned
	 * type is typically the implementing class.
	 */
	virtual Clonable *Clone() const = 0;
};
