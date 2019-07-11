#include "com_imgprocesadondk_ImgProcesadoNDK.h"
#include <android/log.h>
#include <android/bitmap.h>
#define LOG_TAG "libimgprocesadondk"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
} rgba;

/*Conversion a grises por pixel*/
JNIEXPORT void JNICALL Java_com_miinstagramNDK_MiInstagramNDK_convertirGrises
    (JNIEnv *env, jobject obj, jobject bitmapcolor, jobject bitmapgris) {
    AndroidBitmapInfo infocolor;
    void *pixelscolor;
    AndroidBitmapInfo infogris;
    void *pixelsgris;
    int ret;
    int y;
    int x;
    LOGI("convertirGrises");
        if ((ret = AndroidBitmap_getInfo(env, bitmapcolor, &infocolor)) < 0) {
            LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
            return;
        }

        if ((ret = AndroidBitmap_getInfo(env, bitmapgris, &infogris)) < 0) {
            LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
            return;
        }

        LOGI("imagen color :: ancho %d;alto %d;avance %d;formato %d;flags %d",
             infocolor.width, infocolor.height, infocolor.stride,
             infocolor.format, infocolor.flags);
        if (infocolor.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
            LOGE("Bitmap no es formato RGBA_8888 !");
            return;
        }

        LOGI("imagen color :: ancho %d;alto %d;avance %d;formato %d;flags %d",
             infogris.width, infogris.height, infogris.stride,
             infogris.format, infogris.flags);
        if (infogris.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
            LOGE("Bitmap no es formato RGBA_8888 !");
            return;
        }

        if ((ret = AndroidBitmap_lockPixels(env, bitmapcolor, &pixelscolor))
            < 0) {
            LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
        }

        if ((ret = AndroidBitmap_lockPixels(env,bitmapgris,&pixelsgris)) <0){
            LOGE("AndroidBitmap_lockPixels() fallo ! error=%d", ret);
        }
        // modificacion pixeles en el algoritmo de escala grises
        for (y = 0; y < infocolor.height; y++) {
            rgba *line = (rgba *) pixelscolor;
            rgba *grisline = (rgba *) pixelsgris;
            for (x = 0; x < infocolor.width; x++) {
                float output = (line[x].red + line[x].green + line[x].blue)/3; //Aquí se transforma en grises
                if (output > 255) output = 255;
                grisline[x].red = grisline[x].green = grisline[x].blue =
                        (uint8_t) output;
                grisline[x].alpha = line[x].alpha;
            }
            pixelscolor = (char *) pixelscolor + infocolor.stride;
            pixelsgris = (char *) pixelsgris + infogris.stride;
        }
        LOGI("unlocking pixels");
        AndroidBitmap_unlockPixels(env, bitmapcolor);
        AndroidBitmap_unlockPixels(env, bitmapgris);
    }


    //******************************************************************************
/*Conversion a sepia por pixel*/
JNIEXPORT void JNICALL Java_com_miinstagramNDK_MiInstagramNDK_convertirSepia
        (JNIEnv *env, jobject obj, jobject bitmapcolor, jobject bitmapsepia) {
        AndroidBitmapInfo infocolor;
        void *pixelscolor;
        AndroidBitmapInfo infosepia;
        void *pixelssepia;
        int ret;
        int y;
        int x;
        LOGI("convertirSepia");
        if ((ret = AndroidBitmap_getInfo(env, bitmapcolor, &infocolor)) < 0) {
            LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
            return;
        }

        if ((ret = AndroidBitmap_getInfo(env, bitmapsepia, &infosepia)) < 0) {
            LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
            return;
        }

        LOGI("imagen color :: ancho %d;alto %d;avance %d;formato %d;flags %d",
             infocolor.width, infocolor.height, infocolor.stride,
             infocolor.format, infocolor.flags);
        if (infocolor.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
            LOGE("Bitmap no es formato RGBA_8888 !");
            return;
        }

        LOGI("imagen color :: ancho %d;alto %d;avance %d;formato %d;flags %d",
             infosepia.width, infosepia.height, infosepia.stride,
             infosepia.format, infosepia.flags);
        if (infosepia.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
            LOGE("Bitmap no es formato RGBA_8888 !");
            return;
        }

        if ((ret = AndroidBitmap_lockPixels(env, bitmapcolor, &pixelscolor))
            < 0) {
            LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
        }

        if ((ret = AndroidBitmap_lockPixels(env,bitmapsepia,&pixelssepia)) <0){
            LOGE("AndroidBitmap_lockPixels() fallo ! error=%d", ret);
        }
        // modificacion pixeles en el algoritmo de sepia
        for (y = 0; y < infocolor.height; y++) {
            rgba *line = (rgba *) pixelscolor;
            rgba *sepialine = (rgba *) pixelssepia;
            for (x = 0; x < infocolor.width; x++) {
                //***********************
                //RED COMPONENT
                float outputRed = (line[x].red * .393) + (line[x].green * .769) + (line[x].blue * .189);
                if (outputRed > 255) {
                    sepialine[x].red = 255;
                } else {
                    sepialine[x].red = (uint8_t) outputRed;
                }
                //GREEN COMPONENT
                float outputGreen = (line[x].red * .349) + (line[x].green * .769) + (line[x].blue * .189);
                if (outputGreen > 255) {
                    sepialine[x].green = 255;
                } else {
                    sepialine[x].green = (uint8_t) outputGreen;
                }
                //BLUE COMPONENT
                float outputBlue = (line[x].red * .272) + (line[x].green * .534) + (line[x].blue * .131);
                if (outputBlue > 255) {
                    sepialine[x].blue = 255;
                } else {
                    sepialine[x].blue = (uint8_t) outputBlue;
                }
                //***********************
                sepialine[x].alpha = line[x].alpha;
            }
            pixelscolor = (char *) pixelscolor + infocolor.stride;
            pixelssepia = (char *) pixelssepia + infosepia.stride;
        }

        LOGI("unlocking pixels");
        AndroidBitmap_unlockPixels(env, bitmapcolor);
        AndroidBitmap_unlockPixels(env, bitmapsepia);
    }
    //******************************************************************************
    //Poner marco 1 **************************************************************
    /*Marco de grosor indicado, negro*/
    JNIEXPORT void JNICALL Java_com_miinstagramNDK_MiInstagramNDK_ponerMarco1
            (JNIEnv *env, jobject obj, jobject bitmapcolor, jobject bitmapenmarcado) {
        int grosor = 10; //en pixelss
        AndroidBitmapInfo infocolor;
        void *pixelscolor;
        AndroidBitmapInfo infoenmarcado;
        void *pixelsenmarcado;
        int ret;
        int y;
        int x;
        LOGI("ponerMarco1");
        if ((ret = AndroidBitmap_getInfo(env, bitmapcolor, &infocolor)) < 0) {
            LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
            return;
        }

        if ((ret = AndroidBitmap_getInfo(env, bitmapenmarcado, &infoenmarcado)) < 0) {
            LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
            return;
        }

        LOGI("imagen color :: ancho %d;alto %d;avance %d;formato %d;flags %d",
             infocolor.width, infocolor.height, infocolor.stride,
             infocolor.format, infocolor.flags);
        if (infocolor.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
            LOGE("Bitmap no es formato RGBA_8888 !");
            return;
        }

        LOGI("imagen color :: ancho %d;alto %d;avance %d;formato %d;flags %d",
             infoenmarcado.width, infoenmarcado.height, infoenmarcado.stride,
             infoenmarcado.format, infoenmarcado.flags);
        if (infoenmarcado.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
            LOGE("Bitmap no es formato RGBA_8888 !");
            return;
        }

        if ((ret = AndroidBitmap_lockPixels(env, bitmapcolor, &pixelscolor))
            < 0) {
            LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
        }

        if ((ret = AndroidBitmap_lockPixels(env,bitmapenmarcado,&pixelsenmarcado)) <0){
            LOGE("AndroidBitmap_lockPixels() fallo ! error=%d", ret);
        }
        // modificacion pixeles para añadir marco 1. Hay que hacerlo por cada componente de color paraque mantenga los colores originales
        //*************************************************************
        for (y = 0; y < infocolor.height; y++) {
            rgba *line = (rgba *) pixelscolor;
            rgba *negroline = (rgba *) pixelsenmarcado;
            for( x = 0; x < infocolor.width; x++){

                //******
                //RED COMPONENT
                float outputRed = line[x].red;
                if (outputRed > 255) {
                    negroline[x].red = 255;
                } else if( (x > infocolor.width-grosor) || (x < grosor) || ( y >infocolor.height-grosor ) || ( y < grosor ) ){
                    //Pintar negro el pixel
                    outputRed = 0;
                }
                negroline[x].red = (uint8_t) outputRed;
                //GREEN COMPONENT
                float outputGreen = line[x].green;
                if (outputGreen > 255) {
                    negroline[x].green = 255;
                } else if( (x > infocolor.width-grosor) || (x < grosor) || ( y >infocolor.height-grosor ) || ( y < grosor ) ){
                    //Pintar negro el pixel
                    outputGreen = 0;
                }
                negroline[x].green = (uint8_t) outputGreen;
                //BLUE COMPONENT
                float outputBlue = line[x].blue;
                if (outputBlue > 255) {
                    negroline[x].blue = 255;
                } else if( (x > infocolor.width-grosor) || (x < grosor) || ( y >infocolor.height-grosor ) || ( y < grosor ) ){
                    //Pintar negro el pixel
                    outputBlue = 0;
                }
                negroline[x].blue = (uint8_t) outputBlue;
                //***********************
                negroline[x].alpha = line[x].alpha;
                //******
            }
            pixelscolor = (char *) pixelscolor + infocolor.stride;
            pixelsenmarcado = (char *) pixelsenmarcado + infoenmarcado.stride;
        }


        LOGI("unlocking pixels");
        AndroidBitmap_unlockPixels(env, bitmapcolor);
        AndroidBitmap_unlockPixels(env, bitmapenmarcado);
    }

    //******************************************************************************