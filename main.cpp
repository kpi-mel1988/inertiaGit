#include <QCoreApplication>


#include "opencv/cv.h"
#include "opencv/cxcore.h"
#include "opencv/cvaux.h"
#include "opencv/highgui.h"


using namespace std;
using namespace cv;


int calculate_axis(Mat*,float * inertiaOut);
void  calculate_inertia(Mat * imageIn,int axis,float inertiaOut);

int main(int argc, char *argv[])
{

   Mat image(480,640,CV_8U,Scalar::all(0));

   int shift = 0;
   int height = 80;
   int width = 80;

   int axis=0;
   float inertia;


   Point start = Point(100,100);


//   rectangle(image,Point(30,30),Point(300,300),Scalar::all(255),10,1,0);

   Point rectPoint[4];

     rectPoint[0] = start;
     rectPoint[1] = cv::Point(start.x+width,start.y);
     rectPoint[2] = cv::Point(start.x+width+shift,start.y+height);
     rectPoint[3] = cv::Point(start.x+shift,start.y+height);

  line(image,rectPoint[0],rectPoint[1],Scalar::all(50),2);
  line(image,rectPoint[1],rectPoint[2],Scalar::all(50),2);
  line(image,rectPoint[2],rectPoint[3],Scalar::all(50),2);
  line(image,rectPoint[3],rectPoint[0],Scalar::all(50),2);




   const Point* ppt[1] = {rectPoint};
   int nCount = 4;

  fillPoly(image,ppt,&nCount,1,Scalar::all(50),8,0);


  Size ksize;
  ksize.width=9;
  ksize.height=9;



  GaussianBlur(image, image, ksize, -1, 0);
  GaussianBlur(image, image, ksize, -1, 0);


   cvNamedWindow("Rectangle", CV_WINDOW_AUTOSIZE);

       imshow( "Rectangle", image);




     axis = calculate_axis(&image,&inertia);




      cvWaitKey(0);



}


int calculate_axis(Mat * imageIn, float * inertiaOut){

    Mat image = *imageIn;
    Mat SqrMat(480,640,CV_32F);
    SqrMat = image;
    Mat Summat(1,640,CV_32F);
//    SqrMat.col(0).copyTo(Summat);
    float inertia;

    reduce(SqrMat, Summat, 0, CV_REDUCE_SUM, CV_32F);

//    for(int i=0;i<640;i++){
//        for(int j=0;j<480;i++){

//        Summat.at<float>(i,0) +=SqrMat.at<float>(i,j);

//        }
//    }

    double minVal=0, maxVal=1;

    minMaxLoc(Summat,&minVal,&maxVal);


    Summat = Summat/maxVal;

    int minLeft=0,minRight=0;
    int breakLeft=0,breakRight=0;

    for(int i=0;i<470;i++){

        cout<<"Val i"<<i<<" "<<Summat.at<float>(0,i)<<endl;

        if (((Summat.at<float>(0,i)-Summat.at<float>(0,minLeft))<1e-3) && (breakLeft==0)){
          minLeft = i;

        }
        else breakLeft=1;

        if ((abs(Summat.at<float>(0,i)-Summat.at<float>(0,i-1))>1e-3) && (Summat.at<float>(0,i)>1e-3)){
          minRight = i;
        }



    }


    minLeft++;
    int meanVal = (minRight - minLeft)/2;
    float LeftMass=0,RightMass=0;


    cout<<"minLeft"<<minLeft<<endl;
    cout<<"minRigth"<<minRight<<endl;
    cout<<"breakLeft"<<breakLeft<<endl;

    cout<<"meanVal"<<meanVal<<endl;

    int count=0;

    float LeftMassPrev=0,RightMassPrev=0;

    int axis=0;

    while (count<10){


        cout<<"Inside whileLoop"<<endl;



        LeftMass=0;
        RightMass=0;

  for(int i=0;i<meanVal;i++){

    LeftMass += Summat.at<float>(0,meanVal+minLeft-i) * i * i *i;
    RightMass += Summat.at<float>(0,meanVal+minLeft+i) * i * i *i;


    }

//  LeftMass = LeftMass/(meanVal*meanVal);
//  RightMass = RightMass/(meanVal*meanVal);


  cout<<"LeftMass"<<LeftMass<<endl;
  cout<<"LeftMassPrev"<<LeftMassPrev<<endl;


  cout<<"RightMass"<<RightMass<<endl;
  cout<<"RightMassPrev"<<RightMassPrev<<endl;




  if(abs(LeftMass-RightMass)<1e-3){
  axis = meanVal+minLeft;
  cout<<"axis"<<axis<<endl;
  break;
  }
  else if ((LeftMass-RightMass)>1e-3){
  meanVal--;
  }
  else if ((RightMass-LeftMass)>1e-3){
  meanVal++;

  cout<<"End of while loop"<<endl;



  }

if ((abs(LeftMass - LeftMassPrev)<0.5) && (abs(LeftMass - LeftMassPrev)<0.5)) {

    axis = meanVal+minLeft;
    cout<<"axis"<<axis<<endl;
    break;


}


LeftMassPrev = LeftMass;
RightMassPrev = RightMass;


count++;
cout<<"Iter"<<count<<"meanVal"<<meanVal<<endl;


}


    inertia = (LeftMass+RightMass)/2;
    inertiaOut = &inertia;


    cout<<"Inertia"<<inertia<<endl;

    return axis;

}


void  calculate_inertia(Mat * imageIn,int axis,float inertia){






}

