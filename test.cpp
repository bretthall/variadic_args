
#include "variadic_args.h"
#include <iostream>

struct Field
{
   int m_i1;
   int m_i2;
   std::string m_s;

   Field ():
      m_i1 (-1),
      m_i2 (-2),
      m_s ("default")
   {}
};

using Params = Apply<Field>;
auto I1 = MakeModifier<Field, int, &Field::m_i1>();
auto I2 = MakeModifier<Field, int, &Field::m_i2>();
auto S = MakeModifier<Field, std::string, &Field::m_s>();

//--------------------------------------------------------------------------------
// Direct call
// void f (const int i, const Params& params = Params ())
// {
//    std::cout << "i = " << i << ", ";
//    std::cout << "i1 = " << params.m_i1 << ", ";
//    std::cout << "i2 = " << params.m_i2 << ", ";
//    std::cout << "s = '" << params.m_s << "'" << std::endl;
// }

//--------------------------------------------------------------------------------
//Templated
template <typename... Args_t>
void f (const int i, Args_t&&... args)
{
   const auto params = Apply<Field>(std::forward (args)...);
   std::cout << "i = " << i << ", ";
   std::cout << "i1 = " << params.m_i1 << ", ";
   std::cout << "i2 = " << params.m_i2 << ", ";
   std::cout << "s = '" << params.m_s << "'" << std::endl;
}

//--------------------------------------------------------------------------------=
//Forwarded
// void fImpl (const int i, const Apply<Field>& params)
// {
//    std::cout << "i = " << i << ", ";
//    std::cout << "i1 = " << params.m_i1 << ", ";
//    std::cout << "i2 = " << params.m_i2 << ", ";
//    std::cout << "s = '" << params.m_s << "'" << std::endl;
// }

// template <typename... Args_t>
// void f (const int i, Args_t... args)
// {
//    fImpl (i, Apply<Field>(args...));
// }

int main ()
{
   // f (0, Params (I1 (1), I2 (2), S ("test")));
   // f (0, Params (I2 (2), S ("test"), I1 (1)));
   // f (0);
   // f (0, Params (I1 (1)));
   // f (0, Params (I2 (2)));
   // f (0, Params (S ("test")));

   f (0, I1 (1), I2 (2), S ("test"));
   f (0, I2 (2), S ("test"), I1 (1));
   f (0);
   f (0, I1 (1));
   f (0, I2 (2));
   f (0, S ("test"));

   return 0;
}

