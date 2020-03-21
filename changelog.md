# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).


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
- Improve indentation.
- Change an accidentally-replaced "X" back to "x" in comments, for a
  lighter commit diff.

### Removed
- Remove commented `CopyToByDataRef` declaration.
- (RGradient*.*) Remove commented dependency on GBuffer (RImage*.*)
- Remove pseudo-apostraphes ('\92') in RMath.cpp (Meld says, "There was
  a problem opening the file [RMath.cpp]. There was a character encoding
  conversion error and it was needed to use a fallback character"
  [sic]): in "What's the difference between..."; before " dtroy May 5".


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


## [Unreleased] - 2017-09-28
### Added
- RSound_audiere (projects must add "additional linker options":
  `audiere-config --libs` including grave accents)


## [Unreleased] - 2017-09-27
### Added
(Additions below are for cross-platform compatibility.)
- Add base to path in base's project file, so <*.h> (as opposed to "*.h") works--since other projects using base will need to do that since base is not in same folder.
- `size_t` overload of `RString::ToString;` but still throws error so added casts to `int` before sending `size_type` to `RString::ToString`

### Changed
- made main.cpp in base's project folder (the ExpertMultimedia Regression Suite) compatible with changes to engine in same folder:
  - changed lFramesCached to .IFrames()
  - changed CopyToByDataRef CopyTo
  - first param of TypeFast is now an address instead of an object
  - don't use animTest.gbFrame object directly (no longer exists)--instead use animTest.frame_ptr [or first call `GBuffer* this_frame_ptr = animTest.GetFramePtr();`]
  - changed `gbScreen.DrawMass(followMass)` to `camera.DrawBox(&gbScreen, followMass, pixelLightGray, pixelDarkGray)`
- move shading directly to GBuffer to GBuffer (was in Gradient) class to sanitize dependency tree.
- enclose RTemplates.cpp code in ifndef preprocessor
- only use "using" statement in cpp files (not h), so context is clear
  - in h files, specify context wherever removing "using" (especially in RMath.h)
- Move includes (other than each cpp file's own header guard) to h files
  (many were in cpp files).
(The following changes are for cross-platform compatibility and
modernization. See also 2017-09-27 in the namdax changelog.)
- Move linker options `-lmingw32 -mwindows` to new "WindowsRelease"
  target (previously in project linker options).
- Replace `NULL` and `null` with `nullptr`.
- Replace `static const` with `static constexpr`.
- Move uses of `GBuffer` from `RGradient_bgra32.h` to `RImage_bgra32.h`
  to avoid circular dependency (gcc did not like that, and this would've
  required forward declaration of classes on both ends--as opposed to
  only in Gradient as required by an older windows compiler).
- Move `ref_*` defines to somewhere after the class is defined (such as
  in "RTypes.h")
- Use correct case when including pmath.h and pmemory.h.
- Since bonefinity's "main.c" was renamed to "main.cpp", change CFLAGS
  to CXXFLAGS and/or CPPFLAGS so that there is no undefined reference to
  main.
- Check `ifData.is_open()` instead of `ifData!=NULL` since that would be
  impossible (`gcc` shows error, though a Windows compiler does not).
(The following settings are copied from a new codeblocks SDL2 project
for cross-platform compatibility and modernization):
- Project, Properties
  - working directory (for each target): bin (keep path relative)
  - settings specific to Debug target:
    - "Type": "Console application"
    - "Pause when execution ends": yes
  - settings specific to Release targets:
- For "Project," "build options (for all targets)":
  - "Compiler settings," "Compiler Flags":
    - -Wall "Enable all common compiler warnings (overrides many other settings)" in "Compiler settings", "Compiler flags" under Warnings
  - "Other compiler options":
    - `sdl2-config --cflags` (including grave accents)
  - "Linker settings," "Other linker options":
    - `sdl2-config --libs` (including grave acceents) to Project, build options
    - Remove `-lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer`
- add new Debug target with Compiler settings, Compiler flags:
  - `-g`
- Modify "default" target (renamed to "Release" in "Project,"
  "Properties," "Build targets"): "Compiler settings," "Compiler flags"
  - `O2` "Optimize even more (for speed)"
  - `-s` "Strip all symbols from binary (minimizes size)"

### Fixed
- `barrAutoAnimate` was set to `false` instead of `NULL` in
  two instances (gcc throws an error).
- (Rename bad Base.depends file) Fix error `In function `_start':`
  `undefined reference to `main'`
  - I couldn't find where to change CFLAGS to CXXFLAGS (since renamed file from c to cpp) to fix error below (adding -shared linker flag didn't work, neither did Project, Build Options, Compiler settings, #defines, removing main=SDL_main)--see also note by "renamed main.c to main.cpp:

### Removed
- `ref_*` defines for RTypes


## [2015-07-25] - 2015-07-25
(SKIP: rollover to later commit)
- last version where Anim had a gbFrame auto (non-pointer) object


## [Unreleased] - 2014-09-21
- AngleToward should have used `yRel=yDest-ySrc;` (was using `yRel=yDest-xSrc;`)
## [Unreleased] - 2014-07-24
### Fixed
- (MAJOR) comparisons for right and bottom edges should have been <= not < [such as xDest+iWidth<=gbDest.iWidth]


## [Unreleased] - 2014-07-23
### Fixed
- (MAJOR) RMath::Rotate (all overloads) not converting to radians (affects Travel3d and any other methods calling RMath::Rotate)


## [Unreleased] - 2014-07-13
### Fixed
- (MAJOR) Copy constructor exception (occurred on pass by value) caused by calling constructor with "false" option [which causes it not to touch arrbyData] but not setting gbNew.arrbyData=null


## [Unreleased] - 2014-06-22
### Fixed
- IntersectionAndRelationship (all 3 overloads) always returns relationship constants that are non-intersection relationships, even if correct intersection point is returned
- (MAJOR) IntersectionAndRelationship (all 3 overloads) set x and y back to zero after calling  Intersection


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


## [Unreleased] - 2012-02-04
### Fixed
(MAJOR)
- `if (iLineRelationshipType==RMath::IntersectionYes||RMath::IntersectionBeyondSegment) {`
  which is not even really comparing anything on the right side of the "||", and changed it to:
  `if ((iLineRelationshipType==RMath::LineRelationshipIntersectionNotTouchingEndsOfLineB)||iLineRelationshipType==RMath::LineRelationshipIntersectionOutOfRange) {`
-`FPOINT (struct) to FPoint (class)`
- `((isinf(val)&(val)<0))` to `(isinf(val)&&((val)<0))` (big bug fix!)

### Removed
- `bFlipYaw` and `bFlipPitch`


## [Unreleased] - ~2012
### Added
- `(arrpentShot[iShotNow])->iIndex=iShotNow;` in Shoot(iDir) from
  dxman-crossplatform


## [Unreleased] - ~2012
### Added
- Camera PointIsInView method from dxman-crossplatform

### Fixed
- Apply all camera.cpp fixes from dxman-crossplatform (except keep
  SDL_mixer)


## [Unreleased] - 2011-12-31
(Clean up math macros to remove type ambiguity.)
### Added
-`*XOFRTHETA_RAD` methods (by copying `*XOFRTHETA_DEG` &
    removing conversion since "cos" takes radians).

## Changed
- `*XOFRTHETA` to `*XOFRTHETA_DEG` (to match engine's naming)
- `XOFRTHETA_DEG` to `DXOFRTHETA_DEG` since it uses `D180_DIV_PI`
- `?OFRTHETA_RAD` to `RConvert_?OFRTHETA_RAD`

## Removed
- FXOFRTHETA_RAD (since XOFRTHETA_RAD does the overloading anyway when
  it calls "cos")


## [Unreleased] - 2011-12-29)
### Fixed
- Shadows: changed
    `m2dReturnShadow.rectRender.top=IROUNDF(yDownness*fScreenH-(float)m2dReturnShadow.rectOriginal.right/2.0f*m2dReturnShadow.fScale);`
    to
    `m2dReturnShadow.rectRender.top=IROUNDF(yDownness*fScreenH-(float)m2dReturnShadow.rectOriginal.top/2.0f*m2dReturnShadow.fScale);`

### Changed
- moved dwTicksAcquiredOutsideOfGameState_Run to base
- moved MetersToMove* to base (from both DXManSDL & dxman-crossplatform)

### Removed
- `SetRotMaxSpeed` (since used mismatched variables by setting Position
  using Rotation method) -- major fix!
```
void Mass3d::SetRotMaxSpeed(float xSpeed, float ySpeed, float zSpeed) {
  xVel=xSpeed;
  yVel=ySpeed;
  zVel=zSpeed;
}
```


## [Unreleased] - 2011-12-24
### Added
- `MeasureTextByRef_TypeFast` in `RFont` class
  - Change the primary overload of `TypeFast` to account for that.

### Fixed
- `SafeAddWrappedPositiveOnly` in base.cpp: do not return negative (see
  commented code for changes)


## [Unreleased] - 2011-12-24
### Fixed
- (via WinMerge) apply all dxman.cpp fixes from dxman-crossplatform
  (except keep SDL_mixer).


## [Unreleased] - 2011-12-23
### Added
* Show help message at the beginning using default raster RFont.

### Changed
* Make pmath initialize itself:
  ProtoArmor namespace used to expect (example lines from
  f_spatialforegroundremover.cpp):
```
PMathStatic* pmathstatic=NULL;
void SpatialForegroundRemoverFilter::Start() {
	if (pmathstatic==NULL) pmathstatic=new PMathStatic();//PMathStatic pmathstatic;
```
Therefore, I changed usage to  `extern PMathStatic pmathstatic;` in
pmath.h and `PMathStatic pmathstatic;` in pmath.cpp.

### Fixed
- `PlaySounds` keeps looping (only loop for SDL_mixer Mix_Chunk struct
  version).
- Fix errors preventing compile (RetroEngine-like base classes AND
  protoarmor namespace [new open-source code and old code made
  open-source for SpatialForegroundRemover]):
```
changed prototype void Gradient::ShadeAlpha to ShadeAlpha
added 'R' prefix and other filename changes to include statements
changed instances of RConvert_ToString to RString_ToString
changed instances of ipoint*.x to ipoint*.X & same for y to Y
changed char* catch (only occurred in RFile, not DXManSDL, nor dxman-crossplatform, nor any other files in base)
	like:
		catch (char* szExn) {
			bGood=false;
			ShowAndDeleteException(szExn,"OpenWrite",sMsg);
			bExn=true;
	to:
		catch (exception& exn) { bGood=false; ShowExn(exn,"Sprite::Load"); bExn=true;
changed PMath::ROfXY to ROfXY in pmath.h
Add:
	#include <frameworkdummy.h>
	-to RFile.h (for uint etc)
Add:
	using namespace ProtoArmor;
	-for uint, "Console" fake static class, etc
	-to base.h
Changed frameworkdummy calls
	-to C++.NET-compatible syntax from ProtoArmor namespace:
		Console.Error.Write to Console::Error.Write
		Console.Error.Flush to Console::Error.Flush
		File.Exists to File::Exists
	-uses the new SpatialForegroundRemover [ProtoArmor open-source namespace] frameworkdummy
Changed ProtoArmor constants to not conflict with base defines:
	F180_DIV_PI to f180_DIV_PI
	D180_DIV_PI to d180_DIV_PI
```
- Fix warnings (x or no '-' = done):
```
added return statement to:
	bool Anim::GotoFrame(int iFrameX)
	bool Anim::GotoNextFrame()
	Anim::Dump()
	Gradient::Init
	Byter::Save()
	Byter::SetMax(int iSizeTo)
	Sprite::Load
	Sprite::Save
	-AND MANY MORE,
		including:
	ExistsAt, MirrorOnYLine
	--which are big bugfixes!!!
instances of "statement has no effect":
	changed "xDestRel; yDestRel=0;" etc to "xDestRel=0; yDestRel=0;" etc
	--this could be a big load of bugfixes!
removed some unused variables
added Console::Error.WriteLine("NOT YET IMPLEMENTED [methodname]") where [methodname] is method name to sprite.h
"operation on iChannel may be undefined":
	changed ++iChannel to iChannel and put iChannel++ on previous line
-change instances of:
	<float.h> to <cfloat>
	<limits.h> to <climits>
comparison between signed and unsigned types
	e.g. changed instances of:
		for (int i=0;
	to one of the following:
		for (size_t i=0;
		for (Uint32 iNow=0;
added
	#include <cstdlib> //malloc, free etc
	#include <cstring> //memcpy etc (requires cstdlib)
	using namespace std;
	-to files that use malloc, free, memcpy
changed
		PMath::pfDistCache[x][y]=NULL;
	to:
		PMath::pfDistCache[x][y]=0.0f;
changed
	static void ResizePointInfoCache(int iRadius, bool bHorizontalMovement, bool bVerticalMovement);
	to:
	static void ResizePointInfoCache(unsigned int iRadius, bool bHorizontalMovement, bool bVerticalMovement);
		& changed implementation which compares the int to an unsigned one anyway
```


## [Unreleased] - 2011-12-13
### Fixed
- (CppCheck) Change faulty check in `RImage_bgra32`:
  `gbDest.iBytesTotal!=gbDest.iBytesTotal` to
  `gbSrc.iBytesTotal!=gbDest.iBytesTotal`!!!
  - in `EffectMoBlurSimModWidth`, `EffectSkewModWidth`


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
