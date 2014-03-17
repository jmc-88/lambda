#pragma once


/**
 * Custom smart pointer class.
 *
 * \tparam _Type The referenced type.
 */
template<typename _Type>
struct Ptr {
private:
	_Type *m_p;

	/**
	 * The copy constructor is deleted.
	 */
	Ptr(Ptr<_Type> const&) = delete;

	/**
	 * The assignment operator is deleted.
	 */
	void operator = (Ptr<_Type> const&) = delete;

public:
	/**
	 * Main constructor.
	 *
	 * \param a_rrp The pointer that will be managed by this Ptr object. It must
	 * be expiring because the Ptr class is designed to hold a unique pointer.
	 */
	Ptr(_Type *&&a_rrp = nullptr)
		:
	m_p(a_rrp) {}

	/**
	 * Move constructor.
	 *
	 * \param a_rrPtr The expiring Ptr object to move.
	 */
	Ptr(Ptr<_Type> &&a_rrPtr)
		:
	m_p(a_rrPtr.m_p) {
		a_rrPtr.m_p = nullptr;
	}

	/**
	 * Destructor. The destructor uses the `delete` operator on the held
	 * pointer.
	 *
	 * Please, note that this destructor has _not_ been declared virtual in
	 * order to make the Ptr class occupy only the pointer size in memory. Do
	 * **not** subclass Ptr or you gonna leak like there's no tomorrow.
	 */
	~Ptr() {
		delete m_p;
	}

	/**
	 * Move assignment operator.
	 *
	 * \param a_rrPtr The Ptr object to move.
	 * \return A reference to this object.
	 */
	Ptr<_Type> &operator = (Ptr<_Type> &&a_rrPtr) {
		delete m_p;
		m_p = a_rrPtr.m_p;
		a_rrPtr.m_p = nullptr;
		return *this;
	}

	/**
	 * Expiring pointer cast operator. Only applicable to expiring instances.
	 *
	 * This operator detaches the stored pointer.
	 *
	 * \return The stored pointer after detaching it.
	 */
	operator _Type*&& () && {
		_Type *p = m_p;
		m_p = nullptr;
		return move(p);
	}

	/**
	 * Structure dereference operator.
	 *
	 * \return The managed pointer itself, so that members of the pointed object
	 * can be accessed.
	 */
	_Type *operator -> () const {
		return m_p;
	}

	/**
	 * Indirection operator.
	 *
	 * \return A reference to the pointed object.
	 */
	_Type &operator * () const {
		return *m_p;
	}

	/**
	 * `bool` cast operator. It is useful in conditions, such as in the
	 * following example:
	 *
	 * ~~~{.cpp}
	 *	Ptr<int> pi = new int(5);
	 *
	 *	// ...
	 *
	 *	if (pi) {
	 *		std::cout << *pi << endl;
	 *	} else {
	 *		std::cout << "no value" << endl;
	 *	}
	 * ~~~
	 *
	 * \return `false` if the managed pointer is null, `true` if it is not.
	 */
	operator bool () const {
		return !!m_p;
	}

	/**
	 * Detaches the stored pointer from the Ptr object.
	 *
	 * The stored pointer will no longer be managed by this object and it will
	 * not be deleted by the destructor.
	 *
	 * The managed pointer will be set to 0.
	 *
	 * \return The stored pointer.
	 */
	_Type *Detach() {
		_Type *const p = m_p;
		m_p = nullptr;
		return p;
	}
};
