package com.miinstagramNDK;

import android.Manifest;
import android.content.ContentValues;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.hardware.Camera;
import android.net.Uri;
import android.os.Build;
import android.os.Environment;
import android.provider.MediaStore;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ImageView;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

public class MiInstagramNDK extends AppCompatActivity implements View.OnClickListener{

    private String tag = "MiInstagramNDK";
    private Bitmap bitmapOriginal = null;
    private Bitmap bitmapGrises = null;
    private Bitmap bitmapSepia = null;
    private Bitmap bitmapEnmarcado = null;
    private ImageView ivDisplay = null;

    //Para tomar la foto de la cámara
    public static final int REQUEST_CODE_TAKE_PHOTO = 0 /*1*/;
    private String mCurrentPhotoPath;
    private Uri photoURI;
    private int cam_anchura =320;   // resolucion deseada de la imagen (ai es más grande consume demasiada memoria)
    private int cam_altura=240;
    //*******************************

    static {
        System.loadLibrary("imgprocesadondk");
    }
    public native void convertirGrises(Bitmap bitmapIn, Bitmap bitmapOut);
    public native void convertirSepia(Bitmap bitmapIn, Bitmap bitmapOut);
    public native void ponerMarco1(Bitmap bitmapIn, Bitmap bitmapOut);

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_img_procesado_ndk);

        //Cámara y opciones de resilución ****************
        Camera mCamera = Camera.open();
        Camera.Parameters params = mCamera.getParameters();

        /*// Check what resolutions are supported by your camera
        List<Camera.Size> sizes = params.getSupportedPictureSizes();
        // Iterate through all available resolutions and choose one.
        // The chosen resolution will be stored in mSize.
                Size mSize;
                for (Size size : sizes) {
                    Log.i(TAG, "Available resolution: "+size.width+" "+size.height);
                    mSize = size;
                }
            }
            */

        params.setPictureSize(cam_anchura, cam_altura);
        mCamera.setParameters(params);
    //*************************************************
        //Activación por código del permiso de uso de la cámara (Sólo tiene efecto en un Android 6 o superior)
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (checkSelfPermission(Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED)
            {
                ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.CAMERA}, 1);
            }
        }
        //*****************************************************

        Log.i(tag, "Imagen antes de modificar");
        ivDisplay = (ImageView) findViewById(R.id.ivDisplay);
        BitmapFactory.Options options = new BitmapFactory.Options();
        // Asegurar que la imagen tiene 24 bits de color
        options.inPreferredConfig = Bitmap.Config.ARGB_8888;
        bitmapOriginal = BitmapFactory.decodeResource(this.getResources(),
                R.drawable.sampleimage, options);
        if (bitmapOriginal != null)
            ivDisplay.setImageBitmap(bitmapOriginal);

        // Listeners
        ivDisplay.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        //dispatchTakePictureIntent();
    }

    public void dispatchTakePictureIntent(View v) {
        Intent takePictureIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        // Ensure that there's a camera activity to handle the intent
        if (takePictureIntent.resolveActivity(getPackageManager()) != null) {
            // Create the File where the photo should go
            File photoFile = null;
            try {
                photoFile = createImageFile();
            } catch (IOException ex) {
                // Error occurred while creating the File
            }
            // Continue only if the File was successfully created
            if (photoFile != null) {

                ContentValues values = new ContentValues();
                values.put(MediaStore.Images.Media.TITLE, "MyPicture");
                values.put(MediaStore.Images.Media.DESCRIPTION, "Photo taken on " + System.currentTimeMillis());
                photoURI = getContentResolver().insert(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, values);

                //Uri photoURI = FileProvider.getUriForFile(AddActivity.this, "com.example.android.fileprovider", photoFile);

                takePictureIntent.putExtra(MediaStore.EXTRA_OUTPUT, photoURI);
                startActivityForResult(takePictureIntent, REQUEST_CODE_TAKE_PHOTO);
            }
        }
    }

    private File createImageFile() throws IOException {
        // Create an image file name
        String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
        String imageFileName = "JPEG_" + timeStamp + "_";
        File storageDir = getExternalFilesDir(Environment.DIRECTORY_PICTURES);
        File image = File.createTempFile(
                imageFileName,  /* prefix */
                ".jpg",         /* suffix */
                storageDir      /* directory */
        );

        // Save a file: path for use with ACTION_VIEW intents
        mCurrentPhotoPath = image.getAbsolutePath();
        return image;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (requestCode == REQUEST_CODE_TAKE_PHOTO && resultCode == RESULT_OK) {

            try {
                bitmapOriginal = MediaStore.Images.Media.getBitmap(getContentResolver(), photoURI);
                ivDisplay.setImageBitmap(bitmapOriginal);
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }

            /*if (requestCode == REQUEST_CODE_TAKE_PHOTO && resultCode == RESULT_OK) {
                Bundle extras = data.getExtras(); // Aquí es null
                Bitmap imageBitmap = (Bitmap) extras.get("data");
                mPhotoImageView.setImageBitmap(imageBitmap);
            }*/

        }
    }

    //Funciones para transformar la imagen ********************************
    /*public void onResetImagen(View v) {
        Log.i(tag, "Resetear Imagen");
        ivDisplay.setImageBitmap(bitmapOriginal);
    }

    public void onConvertirGrises(View v) {
        Log.i(tag, "Conversion a escala de grises");
        bitmapGrises = Bitmap.createBitmap(bitmapOriginal.getWidth(),
                bitmapOriginal.getHeight(), Bitmap.Config.ARGB_8888);
        convertirGrises(bitmapOriginal, bitmapGrises);
        ivDisplay.setImageBitmap(bitmapGrises);
    }

    public void onConvertirSepia(View v) {
        Log.i(tag, "Conversion a color sepia");
        bitmapSepia = Bitmap.createBitmap(bitmapOriginal.getWidth(),
                bitmapOriginal.getHeight(), Bitmap.Config.ARGB_8888);
        convertirSepia(bitmapOriginal, bitmapSepia);
        ivDisplay.setImageBitmap(bitmapSepia);
    }*/
    //******************************************************************
    //Menu *************************************************************
    @Override public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu, menu);
        return true; /** true -> el menú ya está visible */
    }

    @Override public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        if (id == R.id.original) {
            Log.i(tag, "Resetear Imagen");
            ivDisplay.setImageBitmap(bitmapOriginal);
            return true;
        }
        if (id == R.id.grises) {
            Log.i(tag, "Conversion a escala de grises");
            bitmapGrises = Bitmap.createBitmap(bitmapOriginal.getWidth(),
                    bitmapOriginal.getHeight(), Bitmap.Config.ARGB_8888);
            convertirGrises(bitmapOriginal, bitmapGrises);
            ivDisplay.setImageBitmap(bitmapGrises);
            return true;
        }
        if (id == R.id.sepia) {
            Log.i(tag, "Conversion a color sepia");
            bitmapSepia = Bitmap.createBitmap(bitmapOriginal.getWidth(),
                    bitmapOriginal.getHeight(), Bitmap.Config.ARGB_8888);
            convertirSepia(bitmapOriginal, bitmapSepia);
            ivDisplay.setImageBitmap(bitmapSepia);
            return true;
        }
        if (id == R.id.marco1) {
            Log.i(tag, "Poner marco 1");
            bitmapEnmarcado = Bitmap.createBitmap(bitmapOriginal.getWidth(),
                    bitmapOriginal.getHeight(), Bitmap.Config.ARGB_8888);
            ponerMarco1(bitmapOriginal, bitmapEnmarcado);
            ivDisplay.setImageBitmap(bitmapEnmarcado);
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
    //******************************************************************
}
