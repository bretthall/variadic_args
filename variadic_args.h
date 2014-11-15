#pragma once

#include <utility>

template <typename Field_t, typename Mod_t, typename... Mods_t>
void Apply_ (Field_t& f, Mod_t&& mod)
{
   std::forward (mod) (f);
}

template <typename Field_t, typename Mod_t, typename... Mods_t>
void Apply_ (Field_t& f, Mod_t&& mod, Mods_t&&... mods)
{
   std::forward (mod) (f);
   Apply_ (f, std::forward (mods)...);
}

template <typename Field_t>
struct Apply : public Field_t
{
   Apply ()
   {}
   
   template <typename... Mods_t>
   Apply (Mods_t&&... mods)
   {
      Apply_ (*this, std::forward (mods)...);
   }
};

template <typename Field_t, typename Arg_t, Arg_t Field_t::* arg>
struct ModifierCopy
{
   const Arg_t& m_value;
   
   ModifierCopy (const Arg_t& value):
      m_value (value)
   {}

   void operator()(Field_t& f) const
   {
      f.*arg = m_value;
   }
};

template <typename Field_t, typename Arg_t, Arg_t Field_t::* arg>
struct ModifierMove
{
   Arg_t&& m_value;
   
   ModifierMove (Arg_t&& value):
      m_value (std::move (value))
   {}

   void operator()(Field_t& f)
   {
      f.*arg = std::move (m_value);
   }
};

template <typename Field_t, typename Arg_t, Arg_t Field_t::* arg>
struct ModifierFactory
{
   ModifierCopy<Field_t, Arg_t, arg> operator()(const Arg_t& value)
   {
      return ModifierCopy<Field_t, Arg_t, arg>(value);
   }

   ModifierMove<Field_t, Arg_t, arg> operator()(Arg_t&& value)
   {
      return ModifierMove<Field_t, Arg_t, arg>(std::move (value));
   }
};

template <typename Field_t, typename Arg_t, Arg_t Field_t::* arg>
auto MakeModifier () -> ModifierFactory<Field_t, Arg_t, arg>
{
   return ModifierFactory<Field_t, Arg_t, arg>();
}
