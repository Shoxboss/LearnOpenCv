#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <string>

using namespace cv;
using namespace std;

#define SCALE 0.2
#define GREEN Scalar( 0, 255, 0)
#define WHITE Scalar(255, 255, 255)

int main(int argc, char** argv)
{
    std::string filename = argc >=2 ? argv[1] : "biliard.jpg";

    Mat src = imread( samples::findFile( filename ), IMREAD_COLOR );

    if(src.empty()){
        printf(" Error opening image\n");
        printf(" Program Arguments: [image_name -- default %s] \n", filename);
        return EXIT_FAILURE;
    }


    int height = src.size().height;
    int width = src.size().width;

    Mat gray; 
    Mat small;
    Mat area; 
    Mat line(src.size(), src.type(), GREEN);
    Mat out( src.size(), src.type(),  WHITE); 

    resize(src, small, Size(src.cols*SCALE, src.rows*SCALE), 0, 0, INTER_LINEAR); 

    int nx = small.size().width;
    int ny = small.size().height*1.3;

    Mat msk( small.size(), small.type(),  WHITE);
    Mat mask_for_mask( Size(nx, ny), msk.type(), Scalar(0, 0, 0)); 
    
    int erosion_size = 12;

    circle(mask_for_mask, Point(mask_for_mask.size().width/2+4, mask_for_mask.size().height/2), ny/2+20, Scalar(255,255,255), FILLED);
    resize(mask_for_mask, mask_for_mask, small.size(), 0, 0, INTER_LINEAR);    

    small.copyTo(msk, mask_for_mask);    
    cvtColor(msk, gray, COLOR_BGR2GRAY);
    

    GaussianBlur(gray, gray, Size(15, 15), 0, 0, 4);
    blur(gray, gray, Size(9, 9));
    equalizeHist(gray, gray );
    medianBlur( gray, gray, 3);

    gray.convertTo(gray, -2, 4, 0);
    medianBlur(gray, gray, 3);

    threshold( gray, area, 254, 255, 0);
    resize(area, area, Size(src.cols, src.rows), 0, 0, INTER_LINEAR);    

    Mat elt = getStructuringElement( 2, Size( 7, 7), Point( 3, 3));

    erode( area, area, elt );

    Mat element2 = getStructuringElement( 2, Size( 2, 2 ), Point( 1, 1) );
    
    morphologyEx( area, area, 4, element2 );
    
    Mat tmp, result(src.size(), src.type(), WHITE);

    resize(mask_for_mask, tmp, src.size(), 0, 0, INTER_LINEAR);

    src.copyTo( result, tmp);

    line.copyTo(result, area);

    imshow( "Contrast x2", result );

    // int count = 0;
    // string res = "./result_";
    // for (int count = filename.length()-1; count > 0 || filename[count] != '/' ; count--) { }

    // for (; count < filename.length();) { res += filename[count++]; }
    
    // imwrite(res, result);
    // imwrite("starry_night.png", result);
    
    waitKey();

    return 0;
}



