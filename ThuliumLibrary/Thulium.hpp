//////////////////////////////////////////////////
/// - THULIUM LIB -                            ///
/// Library functions and classes for easy gui ///
/// building in C++.                           ///
///                                            ///
///                                            ///
///                                            ///
/// Built with SFML. SFML provides the basic   ///
/// window and the ability to draw shapes,     ///
/// textures etc. to the screen. All other     ///
/// widget, event, scene-system functionality  ///
/// is built around it in ThuliumLib.          ///
///                                            ///
/// Credits to the creators of SFML.           ///
///                                            ///
/// 2024 / 2025 Julian Benzel                  ///
//////////////////////////////////////////////////
/// Thulium consists of multiple modules
/// 
/// currently available modules are:
/// - ThuliumWindow for creating windows
/// - ThuliumEvents for handling input events
/// - ThuliumWidgets for creating UI-widgets
/// - ThuliumViewport for creating 2D and 3D environment-viewports
/// - ThuliumVFX for adding various visual effects to your UI-widgets
/// 
/// some modules have features with 'hard dependency'
/// to other modules, meaning they will not compile
/// without their dependencies
/// 
/// other modules have features with 'soft dependency'
/// to other modules, meaning that the modules will
/// compile, but the features with dependencies to
/// other modules will be left out
/// 
//////////////////////////////////////////////////
#ifndef THL_LIB
#define THL_LIB
//////////////////////////////////////////////////

#if __has_include("LinAlgpp.hpp")
#include "LinAlgpp.hpp"
#else
#error ThuliumLib is missing dependency: 'LinAlgpp.hpp'
#endif

#if __has_include("ThuliumWindow.hpp")
#include "ThuliumWindow.hpp"
#endif

#if __has_include("ThuliumEvents.hpp")
#include "ThuliumEvents.hpp"
#endif

#if __has_include("ThuliumWidgets.hpp")
#include "ThuliumWidgets.hpp"
#endif

//////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////
