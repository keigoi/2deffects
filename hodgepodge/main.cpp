#include "setup.h"
#include "utils/window.h"
#include "utils/bitmap.h"
#include "utils/eventpumper.h"

#include <iostream>
using std::cin;
using std::cout;

#include <memory>
using std::auto_ptr;

#include <string>
#include <sstream>
using std::string;
using std::stringstream;


#include "hodgepodge.h"




void hodgepodge(int w, int h, int n, double k1, double k2, int g, int framerate)
{
	using namespace syd;
	

	Hodgepodge hp(w,h, n, k1, k2, g);

	// �^�C�g���o�[��Hodgepodge�̃p�����[�^��\��
	stringstream title;
	title << "w:" << w
		<< " h:" << h
		<< " n:" << n
		<< " k1:" << k1
		<< " k2:" << k2
		<< " g:" << g
		<< " fr:" << framerate;

	auto_ptr<Window> window =
		setup::create_window();
	Window &win = *window;

	
	win.create( Rect( 0,0, w,h ))
		.setTitle( title.str().c_str() )
		.show( true );


	Bitmap& winbuf = win.getBuffer();
	EventPumper& evp = win.getEventPumper();


	syd::FixedFramerateKeeper fk(framerate);


	int cellsize=w*h;

	while(!evp.toExit()) { // �I������(���C���E�B���h�E���������EventPumper.toExit()==true)

		hp.proceed();
		{
			int *bmpptr, *cellptr;
			bmp_lock_key a(winbuf); // winbuf(Bitmap)��lock,unlock��������


			winbuf.getPtr(bmpptr);
			cellptr = hp.getPtr();


			for(int i=0;i<cellsize;i++) {
				bmpptr[i]=cellptr[i]+(cellptr[i]<<8)+(cellptr[i]<<16); // �D�F
			}

		}
		
		win.refresh(); // ��ʂ̍X�V
		fk.wait(setup::timer, setup::sleep);  // �t���[�����[�g�Œ菈��
		evp.pumpEvents(); // OS�ɏ�����n��

	}

}



int main(void)
{

	int w=256,h=256,k1=2,k2=3;

	char c;

	do {

		unsigned g=5;
		cout << "g(1-20):";
		cin >> g;

		unsigned framerate=20;
		cout << "Framerate:";
		cin >> framerate;


		hodgepodge(w,h, 100, k1, k2, g, framerate);

	
		cin.clear();
		cout << "continue?(Y/y)";
		cin >> c;

	} while(c=='y' || c=='Y');

	return 0;
	
}



