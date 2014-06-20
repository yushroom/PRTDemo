#include "Light.h"
//#include <opencv2\opencv.hpp>
#include <FreeImage.h>
#include "SHRotate.h"
//using cv::Mat;

// directional light
Light* Light::directLight(Sampler& sampler, int numBands, float lightIntensity)							
{
	//float lightIntensity = 0.4;

	Light* light = new Light(numBands);
	for (unsigned int i = 0; i < light->numFunctions; i++) {
		light->coeffs->r[i] = light->coeffs->g[i] = light->coeffs->b[i] = 0.f;
		for (unsigned int j = 0; j < sampler.size(); j++) {
			if (sampler[j].theta < PI/6.0f) {
				light->coeffs->r[i] += lightIntensity*sampler[j].shValues[i];
				light->coeffs->g[i] += lightIntensity*sampler[j].shValues[i];
				light->coeffs->b[i] += lightIntensity*sampler[j].shValues[i];
			}
		}
		light->coeffs->r[i] *= 4*PI/sampler.size();
		light->coeffs->g[i] *= 4*PI/sampler.size();
		light->coeffs->b[i] *= 4*PI/sampler.size();
	}
	return light;
}

// 6 Lighting
// out: color
void LightProbeAccess(vec3* color, Image* image, vec3& direction)
{
	float d = sqrt(direction.x*direction.x + direction.y*direction.y);
	float r = (d == 0) ? 0.0f : (1.0f/PI/2.0f) * acos(direction.z) / d;
	float tex_coord [2];
	tex_coord[0] = 0.5f + direction.x * r;
	tex_coord[1] = 0.5f + direction.y * r;
	int pixel_coord [2];
	pixel_coord[0] = int(tex_coord[0] * image->width);
	pixel_coord[1] = int(tex_coord[1] * image->height);
	int pixel_index = pixel_coord[1]*image->width + pixel_coord[0];
	color->r = image->pixel[0][pixel_index];
	color->g = image->pixel[1][pixel_index];
	color->b = image->pixel[2][pixel_index];
}

// using light probe (Image Based Lighting)
Light* Light::lightFromImage(const char* imagePath, Sampler& sampler, int numBands, float lightIntensity)
{
	Image img;
	img.loadFromFile(imagePath);

	if (numBands < 3) {
		numBands = 3;
		cout << "[WARNING] bands must be >=3 and <= 10. Set bands = 3.\n";
	} else if (numBands > 10) {
		numBands = 10;
		cout << "[WARNING] bands must be >=3 and <= 10. Set bands = 10\n";
	}

	Light* light = new Light(numBands);
	for (int i = 0; i < numBands*numBands; i++) {
		light->coeffs->r[i] = 0.0f;
		light->coeffs->g[i] = 0.0f;
		light->coeffs->b[i] = 0.0f;
	}


	for (int i = 0; i < sampler.size(); i++) {
		vec3& direction = sampler.samples[i].direction;
		for (int j = 0; j < numBands*numBands; j++) {
			vec3 color;
			LightProbeAccess(&color, &img, direction);
			float sh_function = sampler.samples[i].shValues[j];
			light->coeffs->r[j] += (color.r * sh_function);
			light->coeffs->g[j] += (color.g * sh_function);
			light->coeffs->b[j] += (color.b * sh_function);
			//Color directionLight(0.01f, 0.01f, 0.01f);	// ฒนนโ
			//if (direction.z > 0) {
			//	light->coeffs[j] += directionLight;
			//}
		}
	}
	float weight = 4.0f*PI * lightIntensity;
	float scale = weight / sampler.size();
	for (int i = 0; i < numBands*numBands; i++) {
		light->coeffs->r[i] *= scale;
		light->coeffs->g[i] *= scale;
		light->coeffs->b[i] *= scale;
	}
	return light;
}

void Light::rotateSHCoefficients(float theta, float phi)
{
	//cout << "Light::rotateSHCoefficients(" << theta << ", " << phi << ")\n";
	RotateSHCoefficients(numBands, coeffs->r, rotatedCoeffs->r, theta, phi);
	RotateSHCoefficients(numBands, coeffs->g, rotatedCoeffs->g, theta, phi);
	RotateSHCoefficients(numBands, coeffs->b, rotatedCoeffs->b, theta, phi);
}

bool Image::loadFromFile(const char* filename)
{
	FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(filename, 0);
	FIBITMAP* env = FreeImage_Load(fifmt, filename);
	if (!env) {
		printf("[Error] load image %s failed.\n", filename);
		return false;
	}
	FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(env);

	//env = FreeImage_ConvertTo24Bits(env);
	//env = FreeImage_ConvertToFloat(env);
	FIRGBF* pixels = (FIRGBF*)FreeImage_GetBits(env);
	height =  FreeImage_GetWidth(env);
	width =  FreeImage_GetHeight(env);

	pixel[0] = new float[height * width];
	pixel[1] = new float[height * width];
	pixel[2] = new float[height * width];

	float factor = 0.6f;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int idx = i*width + j;
			pixel[0][idx] = pixels[idx].red * factor;
			pixel[1][idx] = pixels[idx].green * factor;
			pixel[2][idx] = pixels[idx].blue * factor;
		}
	}
	return true;
}