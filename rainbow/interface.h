
// 以下の ifdef ブロックは DLL から簡単にエクスポートさせるマクロを作成する標準的な方法です。 
// この DLL 内のすべてのファイルはコマンドラインで定義された RAYLIB_EXPORTS シンボル
// でコンパイルされます。このシンボルはこの DLL が使用するどのプロジェクト上でも未定義でなけ
// ればなりません。この方法ではソースファイルにこのファイルを含むすべてのプロジェクトが DLL 
// からインポートされたものとして RAYLIB_API 関数を参照し、そのためこの DLL はこのマク 
// ロで定義されたシンボルをエクスポートされたものとして参照します。
#include <windows.h>
#include "raytrace.hpp"

#ifdef RAYLIB_EXPORTS
#define RAYLIB_API __declspec(dllexport)
#else
#define RAYLIB_API __declspec(dllimport)
#endif


#if 0//defined(SWIG) || defined(__SWIG__)
//extern "C"{
void  raytrace_draw(HDC hdc, unsigned w, unsigned h, unsigned depth);

void  photo_env( const geo::vector3d* light_dir, double light_intensity, double ambient,
						   const raytrace::color_t* skycol, int rainbow, double rainbow_intensity );

void  proj_area( const syd::Rect* rect );

void  proj_system( const geo::vector3d* plane_pos, const geo::vector3d* u_dir, const geo::vector3d* v_dir, double camera_dist );
//}

#else

extern "C" RAYLIB_API void __stdcall raytrace_draw(HDC hdc, unsigned w, unsigned h, unsigned depth);

extern "C" RAYLIB_API void  __stdcall photo_env( const geo::vector3d* light_dir, double light_intensity, double ambient,
						   const raytrace::color_t* skycol, int rainbow, double rainbow_intensity );

extern "C" RAYLIB_API void  __stdcall proj_area( const syd::Rect* rect );

extern "C" RAYLIB_API void  __stdcall proj_system( const geo::vector3d* plane_pos, const geo::vector3d* u_dir, const geo::vector3d* v_dir, double camera_dist );
#endif
