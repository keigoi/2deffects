
// �ȉ��� ifdef �u���b�N�� DLL ����ȒP�ɃG�N�X�|�[�g������}�N�����쐬����W���I�ȕ��@�ł��B 
// ���� DLL ���̂��ׂẴt�@�C���̓R�}���h���C���Œ�`���ꂽ RAYLIB_EXPORTS �V���{��
// �ŃR���p�C������܂��B���̃V���{���͂��� DLL ���g�p����ǂ̃v���W�F�N�g��ł�����`�łȂ�
// ��΂Ȃ�܂���B���̕��@�ł̓\�[�X�t�@�C���ɂ��̃t�@�C�����܂ނ��ׂẴv���W�F�N�g�� DLL 
// ����C���|�[�g���ꂽ���̂Ƃ��� RAYLIB_API �֐����Q�Ƃ��A���̂��߂��� DLL �͂��̃}�N 
// ���Œ�`���ꂽ�V���{�����G�N�X�|�[�g���ꂽ���̂Ƃ��ĎQ�Ƃ��܂��B
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
