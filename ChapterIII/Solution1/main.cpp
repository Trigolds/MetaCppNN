// =============================================================================
//                    MetaProg Chapter III Solution 1
//                         Author : Xin Liu
// =============================================================================

#include <iostream>
#include <limits>
#include <type_traits>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/multiplies.hpp>


namespace mpl = boost::mpl;

////////////////////////////////////////////////////////////////////////////////
namespace dimension_analysis
{
    using mass                  = mpl::vector_c<int, 1, 0, 0, 0, 0, 0, 0>;
    using length                = mpl::vector_c<int, 0, 1, 0, 0, 0, 0, 0>;
    using time                  = mpl::vector_c<int, 0, 0, 1, 0, 0, 0, 0>;
    using charge                = mpl::vector_c<int, 0, 0, 0, 1, 0, 0, 0>;
    using temperature           = mpl::vector_c<int, 0, 0, 0, 0, 1, 0, 0>;
    using intensity             = mpl::vector_c<int, 0, 0, 0, 0, 0, 1, 0>;
    using amount_of_substance   = mpl::vector_c<int, 0, 0, 0, 0, 0, 0, 1>;

    using velocity              = mpl::vector_c<int, 0, 1, -1, 0, 0, 0, 0>; // l/t
    using acceleration          = mpl::vector_c<int, 0, 1, -2, 0, 0, 0, 0>; // l/(t^2)
    using momentum              = mpl::vector_c<int, 1, 1, -1, 0, 0, 0, 0>; // ml/t
    using force                 = mpl::vector_c<int, 1, 1, -2, 0, 0, 0, 0>; // ml/(t^2)

    using scalar                = mpl::vector_c<int, 0, 0, 0, 0, 0, 0, 0>;


    template<typename T, typename Dimensions>
    class quantity
    {
    public:
        explicit quantity(T x)
                : m_value(x)
        { }

        template <typename OtherDimensions>
        quantity(quantity<T, OtherDimensions> const& q)
                : m_value(q.value())
        {
            static_assert(mpl::equal<Dimensions, OtherDimensions>::type::value, "OtherDimensions should be same as Dimensions.");
        }

    public:
        T value() const
        {
            return m_value;
        }

    private:
        T m_value;
    };

    template <typename T, typename D>
    quantity<T, D>
    operator+ (const quantity<T, D>& lhs, const quantity<T, D>& rhs)
    {
        //static_assert(mpl::equal<D1, D2>(), "D1 and D2 dimensions should be same.");
        return quantity<T, D>(lhs.value() + rhs.value());
    }

    template <typename T, typename D>
    quantity<T, D>
    operator- (const quantity<T, D>& lhs, const quantity<T, D>& rhs)
    {
        //static_assert(mpl::equal<D1, D2>(), "D1 and D2 dimensions should be same.");
        return quantity<T, D>(lhs.value() - rhs.value());
    }

    using mpl::placeholders::_;

    template <typename D1, typename D2>
    struct multiply_dimensions
            : mpl::transform<D1, D2, mpl::plus<_, _> >
    { };

    template <typename T, typename D1, typename D2>
    quantity<T, typename multiply_dimensions<D1, D2>::type>
    operator* (const quantity<T, D1>& lhs, const quantity<T, D2>& rhs)
    {
        return quantity<T, typename multiply_dimensions<D1, D2>::type>(lhs.value() * rhs.value());
    }

    template <typename D1, typename D2>
    struct divide_dimensions
            : mpl::transform<D1, D2, mpl::minus<_, _>>
    { };

    template <typename T, typename D1, typename D2>
    quantity<T, typename divide_dimensions<D1, D2>::type>
    operator/ (const quantity<T, D1>& lhs, const quantity<T, D2>& rhs)
    {
        return quantity<T, typename divide_dimensions<D1, D2>::type>(lhs.value() / rhs.value());
    }
} // namespace dimension_analysis


int main()
{
    using namespace dimension_analysis;

    quantity<float, length> l{ 1.0f };
    quantity<float, mass> m{ 2.0f };

    // expected compile error
    // m = l;

    quantity<float, length> len1{ 1.0f };
    quantity<float, length> len2{ 2.0f };

    len1 = len1 + len2;

    // expected compile error
    //len1 = len2 + quantity<float, mass>{ 3.7f };

    quantity<float, mass> m1{ 5.0f };
    quantity<float, mass> m3{ 10.0f };
    quantity<float, acceleration> a1{ 9.8f };

    quantity<float, force> f1 = m1 * a1;

    quantity<float, mass> m2 = f1 / a1;
    float rounding_error = std::abs((m2 - m1).value());
    std::cout << "error: " << rounding_error << "\n";

    f1 = f1 + (quantity<float, force>)(m3 * a1);
    float rounding_error2 = std::abs((m2 - m1).value());
    std::cout << "error2: " << rounding_error2 << "\n";
    bool res = rounding_error2 <= std::numeric_limits<float>::min(); 
    if(res) return 0;
    else return -1;
}
