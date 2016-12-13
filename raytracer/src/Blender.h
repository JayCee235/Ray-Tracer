#ifndef __BLENDER
#define __BLENDER

#include "buffer/Buffer.h"
#include "vector/GenVector.h"

class Blender
{
public:
	Blender() {};
	~Blender() {};

	Buffer<Vector3>* blend(Buffer<Vector3>* in, int scale) {
		int sx = in->getWidth();
		int sy = in->getHeight();

		Buffer<Vector3>* out = new Buffer<Vector3>(sx / scale, sy / scale);

		for(int i = 0; i < out->getWidth(); i++) {
			for(int j = 0; j < out->getHeight(); j++) {
				Vector3 mix = Vector3(0, 0, 0);
				for(int x = 0; x < scale; x++) {
					for(int y = 0; y < scale; y++) {
						mix = mix + in->at(i*scale+x, j*scale+y);
					}
				}
				out->at(i, j) = mix;
			}
		}

		return out;
	}

	Buffer<Vector3>* scaleDown(Buffer<Vector3>* in, int scale) {
		int sx = in->getWidth();
		int sy = in->getHeight();

		Buffer<Vector3>* out = new Buffer<Vector3>(sx / scale, sy / scale);

		for(int i = 0; i < out->getWidth(); i++) {
			for(int j = 0; j < out->getHeight(); j++) {
				int si = scale/2;
				int sj = scale/2;
				out->at(i, j) = in->at(i*scale + si, j*scale+sj);
			}
		}

		return out;
	}

	Buffer<Color>* bufferToImage(Buffer<Vector3>* in) {
		float max = 0;
		for(int x = 0; x < in->getWidth(); x++) {
			for(int y = 0; y < in->getHeight(); y++) {
				Vector3 work = in->at(x, y);
				if(work[0] > max) {
					max = work[0];
				}
				if(work[1] > max) {
					max = work[1];
				}
				if(work[2] > max) {
					max = work[2];
				}
			}
		}
		if(max == 0) max = 0.0000001;

		Buffer<Color>* out = new Buffer<Color>(in->getWidth(), in->getHeight());
		for(int i = 0; i < in->getWidth(); i++) {
			for(int j = 0; j < in->getHeight(); j++) {
				Vector3 v = in->at(i, j);
				Color c = Color(v[0]*255.0f/max, v[1]*255.0f/max, v[2]*255.0f/max);
				out->at(i, j) = c;
			}
		}
		return out;
	}
};

#endif