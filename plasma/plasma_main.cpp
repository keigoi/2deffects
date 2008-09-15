#include <memory>
#include <iostream>
#include <iomanip>
#include <fstream>

#include <ctime>
#include <cmath>

#include "utils/bitmap.h"
#include "utils/window.h"
#include "utils/color.h"
#include "utils/types.h"

#include "utils/plainbmp.h"

#include "plasma_main.hpp"


void _BLUR32_(void* in, void* out, int w, int h)
{
//assert sizeof(char)==1...
	int size=4;
	char *i=(char*)in+w*size;
    char *o=(char*)out;
	char *up=i-(w*size),*down=i+(w*size);
	int  m=w*(h-2);//4byte分で1ループ

__asm{
    mov edx,i;
    mov esi,up;
    mov edi,down;
    mov ecx,m;
	push ebp;
	mov ebp,o; //これ以降大概のコンパイラではpop ebpまで自動変数は使えない。
    }
//	ecx:ループカウンタ
//	eax:汎用
//	ebx:汎用
//	edx:in
//	esi:up
//	edi:down
loop:
__asm{
//1
    xor ebx,ebx;
    xor eax,eax;

    mov bl,byte ptr[edx+4];
    mov al,byte ptr[edx-4];

    add eax,ebx;
    mov bl,byte ptr[esi];

    inc esi;
    add eax,ebx;

    mov bl,byte ptr[edi];
    inc edi;

	add eax,ebx;
	inc edx;

    sar eax,2;
    mov byte ptr[ebp],al;

	inc ebp;

//2
    xor ebx,ebx;
    xor eax,eax;

    mov bl,byte ptr[edx+4];
    mov al,byte ptr[edx-4];

    add eax,ebx;
    mov bl,byte ptr[esi];

    inc esi;
    add eax,ebx;

    mov bl,byte ptr[edi];
    inc edi;

	add eax,ebx;
	inc edx;

    sar eax,2;
    mov byte ptr[ebp],al;

	inc ebp;

//3
    xor ebx,ebx;
    xor eax,eax;

    mov bl,byte ptr[edx+4];
    mov al,byte ptr[edx-4];

    add eax,ebx;
    mov bl,byte ptr[esi];

    inc esi;
    add eax,ebx;

    mov bl,byte ptr[edi];
    inc edi;

	add eax,ebx;
	inc edx;

    sar eax,2;
    mov byte ptr[ebp],al;

	inc ebp;
//4
	inc esi;
    inc edi;
    inc edx;
    inc ebp;

	dec ecx;

	jnz loop;

	pop ebp;
}
}

std::auto_ptr<syd::PlainBmp> 
prepare_table(unsigned w, unsigned h, TableCalcFunc func) {

	using namespace syd;
	std::auto_ptr<PlainBmp> pbmp(new PlainBmp(w, h, 1));
	bmp_lock_key key___(*pbmp);

	uint8_t* ptr = pbmp->getPtr(ptr);

	for(unsigned y=0; y<h; y++)	{
		for(unsigned x=0; x<w; x++) {
			ptr[x+y*w]= (*func)(x, y, w, h);
		}
	}

	return pbmp;
}

void 
plasma_draw(syd::Bitmap& bmp, syd::Bitmap& table, unsigned time) {
    
	unsigned w = bmp.width(), 
			 h = bmp.height();

	float t = (float)time;

	unsigned // 適当なパラメータ達
		x1=(unsigned)((w/2) * (1.0 + cos( t*0.05))),
		y1=(unsigned)((h/2) * (1.0 + sin(-t*0.03333))),
		x2=(unsigned)((w/2) * (1.0 + sin( t*0.01412))),
		y2=(unsigned)((h/2) * (1.0 + cos(-t*0.01666))),
		x3=(unsigned)((w/2) * (1.0 + cos( t*0.06666))),
		y3=(unsigned)((h/2) * (1.0 + sin(-t*0.025))),
		x4=(unsigned)((w/2) * (1.0 + cos( t*0.05833))),
		y4=(unsigned)((h/2) * (1.0 + sin(-t*0.00833))),
		roll = (unsigned)t*5;

	// lock/unlock を 自動的に行う「鍵」
	syd::bmp_lock_key key1___(bmp);
	syd::bmp_lock_key key2___(table);

	// 書き込み先bmp
	syd::color_t *topLeft = bmp.getPtr(topLeft);
	syd::color_t *pixel = topLeft; 

	// 元テーブル
	uint8_t *tab = table.getPtr(tab);
	uint8_t *t1=tab + x1+y1*w*2,
			*t2=tab + x2+y2*w*2,
			*t3=tab + x3+y3*w*2,
			*t4=tab + x4+y4*w*2;

	for(unsigned y=0; y<h; y++) {
		for(unsigned x=0; x<w; x++) {

		assert(pixel-topLeft<w*h);
		assert(t1-tab<w*h*4);
		assert(t2-tab<w*h*4);
		assert(t3-tab<w*h*4);
		assert(t4-tab<w*h*4);
		// this is the heart of the plasma
		//4種類のプラズマ
#define TEST9_2 // CHANGE HERE TO SEE VARIOUS KIND OF PLASMA -- Keigo IMAI
#if   defined(TEST9_1) // RED NORMAL PLASMA
		  	pixel->r=*t1+roll+*t2+*t3+*t4; 
#elif defined(TEST9_2) // MY FAVOURITE COLOUR
		  	pixel->b=*t1+roll+*t2+*t4;
		  	pixel->r=*t2+roll+*t3;
			pixel->g=(*t2+roll+*t3+*t4)>>1; 
#elif defined(TEST9_3) // GREEN MUD
			pixel->g+=(*t2+roll+*t3+*t4)>>1; 
#elif defined(TEST9_4) // HOW TO MAKE A PLASMA
		  	pixel->r=*t4;
#endif
            ++pixel;
            ++t1;
            ++t2;
            ++t3;
            ++t4;
		}
        t1+=w;
        t2+=w;
        t3+=w;
        t4+=w;
	}
}

