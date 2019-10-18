#pragma once
#include <cstdlib>
#include "restricted_ref_counter.hpp"

/*
	Implement a class RestrictedPtr that holds a pointer of any type with at least the following members:
		- default constructor (takes no parameters)
		- constructor with a raw pointer parameter
		- copy constructor
		- destructor
		- copy assignment operator
		- function GetData that takes no parameters and returns the data the raw pointer refers to.
			The data returned should be such that modifying it modifies the data the pointer refers to.
		- function GetPointer that takes no parameters and returns the raw pointer
		- function GetRefCount that takes no parameters and returns an int with the reference count
	
	At any time the objects should keep track of how many references there are to a pointer
	and not allow more than 3 references. If the reference count drops to 0, the referenced
	pointer's memory should be released. If the reference count is already 3 when copying,
	the copier should set it's pointer to nullptr and reference count to 1.
	NOTE: GetPointer, GetData and GetRefCount are needed for the tests to run
*/

template <typename T>
class RestrictedPtr
{
	public:

		//RestrictedPtr() : m_ptr_(nullptr), m_count_(0) {};
		~RestrictedPtr()
		{
			m_count_->Decrement();
			//if( m_count_->GetCount() == 0 )
			//{
				std::cout << "Hi\n";
				std::free(m_ptr_);
				std::free(m_count_);
			//}
		}
		
		RestrictedPtr(T* ptr = nullptr)
		{
			if(ptr != nullptr)
			{
				m_count_ = new RestrictedRefCounter();
				m_ptr_ = ptr;
				std::cout << "From cns\n";
				//m_count_->Increment();
			}
		}

		RestrictedPtr(const RestrictedPtr<T>& rp)
		{
			std::cout << "Hello from dd\n";
			if( m_ptr_ != nullptr )
			{
				std::cout << "a7a\n";
				m_count_->Decrement();

				if( m_count_->GetCount() < 1 ){
					std::free(m_ptr_);
					std::free(m_count_);
				}
			}
			if( !rp.CountInc() )
			{
				m_ptr_ = nullptr;
			}
			else
			{
				m_ptr_ = rp.GetPointer();
				m_count_ = GetCounter();
			}
		}


		T* GetPointer(void) const { return m_ptr_; }
		T& GetData(void) {return (*m_ptr_); }
		int GetRefCount(void) const { return m_count_->GetCount(); }
		RestrictedRefCounter* GetCounter(void) const { return m_count_; }
		bool CountInc(void) const { return m_count_->Increment(); }

		void operator = (const RestrictedPtr<T>& rp)
		{
			std::cout << "Hi from op\n";
			if( !rp.CountInc() )
			{
				m_ptr_ = nullptr;
			}
			else
			{
				if( m_ptr_ != nullptr )
				{
					m_count_->Decrement();

					if( m_count_->GetCount() < 1 ){
						std::free(m_ptr_);
						std::free(m_count_);
					}
				}
				
				m_count_ = rp.GetCounter();
				m_ptr_ = rp.GetPointer();
			}
		}

	private:

		T* m_ptr_;
		RestrictedRefCounter* m_count_;
};

