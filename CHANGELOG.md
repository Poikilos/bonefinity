# Changelog
(see also "Changes to ExpertMultimediaBase" in namdax)


## [Unreleased] - 2018-12-11

### Added
- require boost (path operations)
- detect data directory (must have images subdirectory)
- add `wait-sweep-openings*.tga`, a "waiting" animation (`spin*.tga` wasn't useful so it was never committed)
- implement `Exists` & `IsDir` static methods in `Byter`

### Changed
- (RAnim_bgra32.cpp) detect frame count; no longer warn if `iFrameCountToUseAndModify<=0`
- (main.cpp) improve spacing of comments


## [frameworkdummy update 2018-12-11 (last non-git copy)] - 2018-12-11
(seems to be based on "2014-04-15"--is mostly the same but adds `namespace System`)

### Changed
- correctly declares Convert methods as static, and after namespace System, `using namespace System;` is used
- incorrectly defines Now as `extern DateTime DateTime_Now;` using underscore (frameworkdummy.h)
- incorrectly defines `System_Math_*` methods using underscores (frameworkdummy.h)
- defines `float_IsNegativeInfinity` (the way C++.NET defines this is unknown)
- correctly defines `static constexpr double PI` in the Math class (frameworkdummy.h)
- defines `Rectangle` and `RectangleF`
- defines C#-like out_* and ref_* for builtin and additional types


## [2017-12-02 redacted] - 2017-12-02
(edited 2018-12-11--same as uncommitted 2017-12-02 except...)

### Changed
- improved indentation
- changed accidentally-replaced "X" back to "x" in comments, for lighter diff
- removed commented: CopyToByDataRef declaration
- (RGradient*.*) remove commented dependency on GBuffer (RImage*.*)
- removed pseudo-apostraphes ('\92') in RMath.cpp (Meld says, "There was a problem opening the file [RMath.cpp]. There was a character encoding conversion error and it was needed to use a fallback character" [sic]): in "What's the difference between..."; before " dtroy May 5"


## [2017-12-02 last non-git copy] - 2017-12-02:
(SKIP: rollover to later commit; seems to be based on "2014-04-15" but has some changes and additions)

### Added
- adds `Mass3D::CopyTo([]...)` methods
- `Travel3d` implemented as methods of Mass3D; function versions kept in base.*
- implements `get_scale_by_percentage_of_smallest_screen_axis`
- create `almost_equal` function template (uses `std::numeric_limits<T>::epsilon()` not `epsilon_tweaked_float`)
- (RImage_bgra32.cpp) implement text margins (see Text_SetMargins overloads and methods for each side)
- (RImage_bgra32.cpp) implement DrawExclusiveRect
- (RImage_bgra32.cpp) implement FillAlpha
- (RAnim_bgra32.*) add safe accessors for current frame's format variables; implement `reserve` to create blank frames
- 2017-12-01: changed sound system (RSound) to dummy (RSound_dummy.*)
- 2017-12-02: changed sound system to Audiere

### Changed
- 2017-11-27: last saved project metadata (Base.*) for test program; worked all files (*.cpp and *.h) except StringQ.* and ones edited following days (below)
- 2017-11-28: worked on Rimage_bgra32.h, RMath.*, base.h, frameworkdummy.*, pmath.cp, RFile.cpp, RGradient_bgra32.cpp, Targa.*
- uses `nullptr` for C++11 (`NULL` is deprecated, and `nullptr` is better since it is a self-defined type that can't be cast to an integer)
- `SafeFree` functions now have more debugging, including DebugNote
- prefers pointers over references for params
- move ShadeAlpha from Gradient (RGradient*.*) to GBuffer (RImage*.*)
- move all includes (except own `*.h` file) from `*.cpp` to `*.h`
- move uses of `using namespace` from `*.h` files to `*.cpp` files as per best practices (easier reading and compile-time debugging)
- change uses of quotes to signs for includes as per best practices (avoid pathing issues when other projects include bonefinity)
- (Rimage_bgra32.cpp) change last param for targaLoaded.From to false
- (RImage_bgra32.cpp) improve `DrawModeCopyAlpha` in `DrawToLargerWithoutCropElseCancel`
- (RImage_bgra32.cpp) check for `nullptr` dest and improve logic in `RawOverlayNoClipToBig`, `OverlayNoClipToBig`, `OverlayNoClipToBigCopyAlpha` (and additional changes in the file, also related to changing parameters from references to pointers)
- (frameworkdummy.*) `Convert` moved to `System` namespace
- change uses of by3dAlphaLookup to APPROACH
- specify `RMath::` explicitly for easier reading
- change case of all-caps (formerly inline) functions such as RMath::XOfRTheta_Rad

### Removed
- removes `/* inline */` comments
- (RGradient_bgra32.*) remove dependency on GBuffer (RImage*.*); add shading method with primitive data types
- (RTypes.*) replaced `FDist` with RMath Dist overload
- (redacted) deleted commented uses of catching `char*` exceptions (Standard Library throws subclasses of std::exception)


## [2015-07-25] - 2015-07-25
(SKIP: rollover to later commit)
- last version where Anim had a gbFrame auto (non-pointer) object


## [frameworkdummy update 2014-04-15] - 2014-04-15
(seems to be based on "2010-07-13 SpatialForegroundRemover" version--is mostly the same but has additional Math features)

### Added
- implement DateTime class

### Changed
- ProtoArmor namespace renamed to ExpertMultimediaBase


## [2014-04-15 redacted] - 2014-04-15
(identical to uncommitted 2014-04-15 version except...)

### Changed
- accidentally-replaced "X" back to "x" in comments, for lighter diff
- changed `Console.` to `Console::` in main.c
- changed pseudo-apostraphe ('\92' in "What's the difference between...") to real apostraphe

### Removed
- (RTypes.*) removed commented version of IPoint; improved indentation


## [2014-04-15] - 2014-04-15
(SKIP: rollover to later commit)

### Added
- implements `Base_GetTicks_Absolute` to `Base_GetSeconds_Absolute` and others in between
- implements `MetersToMoveThisManyMS` to `DegreesToMoveThisManyS` and other in between

### Changed
- Move code from namdax, fix debug bugs, add more debug&math. Also change namespace; explicitly use RMath; switch to `exception`; specify _RAD for angle functions; fix undefined value and missing return bugs
- Uses RString_ToString
- Uses uppercase for coordinate members
- Uses `Console::`
- Uses `size_t` variables in `GetForcedCSVNotationSubstring`


## [2010 redacted2 (split base like 2014-04-15)] - 2010-10-27
(same as "2010 redacted" but split up like 2014-04-15 version; This is not the next commit to which "2010 redacted" refers--rather, 2014-04-15 is.)

### Added
- adds but doesn't modify: pixelinfo.*, pmath.*, pmemory.*, pointinfo.*, preporting.*

### Changed
- splits base.* into: base.*, RTypes.* (content in camera.*, RMath.*, RTemplates.*: some came from namdax, some from SpatialForegroundRemover)


## [2010 redacted (for improved diff with next commit)] - 2010-10-27

### Changed
- frameworkdummy from "2010-07-13"
- changed Console. to Console::
- base from "2010-10-27 uppercase coordinate members"
- everything else from "2010-02-07"


## [2010-10-27 redacted] - 2010-10-27
(identical to uncommitted 2010-10-27 version except...)

### Changed
- same as 2010-10-27 except changed naming to be more like previous and next version


## [2010-10-27] - 2010-10-27
(SKIP this: rollover to later commit)

### Changed
- Uses RConvert_ToString instead of RString_ToString
- Uses uppercase for coordinate members


## [frameworkdummy update 2010-07-13 SpatialForegroundRemover] - 2010-07-13
(Missing `System::Math::*`, `DateTime::Now` object [see last ownCloud version; for alternative see <https://www.boost.org/doc/libs/1_45_0/doc/html/date_time/date_time_io.html>; for C++/CLI usage example see <https://stackoverflow.com/questions/41008578/c-cli-equivalent-of-cs-datetime-now-ticks>: `DateTime::Now.Ticks;`], `Math::PI`, `float_IsNegativeInfinity` and other type as object methods, `SizeF`, defines for C#-like out_* and ref_* for builtin and additional types; Convert is missing `static int ToInt32(string val);`)

### Added
- defines `Rectangle` and `RectangleF`
- defines `Stream`, `FileStream`, `BinaryReader`, `BinaryWriter`

### Changed
- Convert methods are static, but Convert is in ProtoArmor namespace and should not be.


## [frameworkdummy update 2010-02-07] - 2010-02-07
(only Console and File, and the methods are not static; incorrectly defines Console and File as objects of type `CONSOLE_FAKESTATIC_CLASS` and `FILE_FAKESTATIC_CLASS` so must use like `Console.` and `File.`; Missing `Convert` `Stream`, `FileStream`, `BinaryReader`, `BinaryWriter`)

### Added
- defines C#-like out_* and ref_* for builtin and additional types


## [frameworkdummy update 2011-12-23 ("pre-SpatialForegroundRemover")] - 2010-02-07
(SKIP this version since identical to 2010-02-07 other than capitalization of some coordinate params, file modification date must not be accurate)


## [2010-02-07] - 2010-02-07
(based on a deprecated C# version [via code conversion to C++/CLI probably using icsharpcode's old online or offline converter--this was followed by manual repair] from unknown date [all files from matching copy of the C# version are dated 2009-02-16]; Uses `Console.`; Uses `int` variables in `GetForcedCSVNotationSubstring`)
