#pragma once
/* Add include guards here */

/* Class: Triple
 * -------------
 * The template class Triple is a container class, which holds three values.
 * All of the values can be of different type. All of the values should be
 * private.
 *
 * The constructor of the class takes three const references to values as a
 * parameter. The parameters are in order, in other words, after construction
 * the first parameter should be accessible through the public member function
 * First, the second parameter through the public member function Second, and
 * the third through the public member function Third. 
 *
 * The default empty constructor should also work.
 *
 * The Triple class should also support the copy constructor, and assignment
 * operations.
 */

/* Public member functions:
 * ------------------------
 * The Triple class has three public member functions, which are used to
 * access the stored elements. All three should work with const instances
 * of Triple.
 *
 * Function: First
 * The function takes no parameters.
 * The function returns a const reference to the first value of the triple.
 *
 * Function: Second
 * The function takes no parameters.
 * The function returns a const reference to the second value of the triple.
 *
 * Function: Third
 * The function takes no parameters.
 * The function returns a const reference to the third value of the triple.
 */

template <typename T1, typename T2, typename T3>
class Triple
{
    public:
        
        Triple() = default;

        Triple(T1 m_1, T2 m_2, T3 m_3) : m_1_(m_1), m_2_(m_2), m_3_(m_3) {};
        //{
        //    m_1_ = m_1;
        //    m_2_ = m_2;
        //    m_3_ = m_3;
        //}

        const T1& First(void) const
        {
            return m_1_;
        }
        const T2& Second(void) const
        {
            return m_2_;
        }
        const T3& Third(void) const
        {
            return m_3_;
        }

        void operator=(const Triple &triple ) { 
            m_1_ = triple.m_1_;
            m_2_ = triple.m_2_;
            m_3_ = triple.m_3_;
        }

        ~Triple() = default;
    private:
        T1 m_1_;
        T2 m_2_;
        T3 m_3_;
};

//template <typename T1, typename T2, typename T3>
//bool IsHomogenous(Triple<T1,T2,T3> t)
//{
//    if(typeid(T1) == typeid(T2) && typeid(T1) == typeid(T3))
//        return true;
//    else
//        return false;
//}

template <typename T1, typename T2, typename T3>
bool IsHomogenous(Triple<T1,T2,T3> t)
{
    return false;
}

template <typename T>
bool IsHomogenous(Triple<T,T,T> t)
{
    return true;
}

template <typename T1, typename T2, typename T3>
bool operator == (const Triple<T1,T2,T3>& t1, const Triple<T1,T2,T3>& t2)
{
    return (t1.First() == t2.First()    &&
            t1.Second() == t2.Second()  &&
            t1.Third() == t2.Third());
}

template <typename T1, typename T2, typename T3>
bool operator != (const Triple<T1,T2,T3>& t1, const Triple<T1,T2,T3>& t2)
{
    return !(t1 == t2);
}
template <typename T1, typename T2, typename T3>
std::ostream& operator << (std::ostream& out, const Triple<T1,T2,T3>& t)
{
    out << "[" << t.First() << ", " << t.Second() << ", " << t.Third() <<"]";
    return out;
}


/* Non-member functions:
 * ---------------------
 * Function: IsHomogenous
 * The function is a non-member function that takes a Triple as a parameter.
 * The function returns a boolean value indicating whether the triple is
 * homogenous or not. A triple is homogenous if all of its three values are of
 * the same type.
 * Hint: specialisation can help you here.
 */

/* Operator overloads:
 * -------------------
 * The triple class should support the following operators:
 *
 * Assignment operator, =
 * The default functionality should do here.
 *
 * Equality operator, ==
 * You can assume that all the values in the triple can be compared by the
 * == -operator. Two triples are equal if the corresponding values in both
 * of the triples are equal.
 *
 * Inequality operator, !=
 * You can assume that all the values in the triple can be compared by the
 * != -operator. Two triples are inequal if even one corresponding value in
 * the triples is different.
 *
 * The output stream operator, <<
 * You can assume that all the values in the triple support the << -operator.
 * The triple should be printed to the output stream in the following format: 
[<first value>, <second value>, <third value>]
 * Note that there is NO new line at the end of the output.
 */

/* Remember the endif here to close the header guards */

