/*
Ctrl + P to Run program from task dependencies
*/

#include <opencv2/highgui/highgui.hpp>  
 #include <opencv2/imgproc/imgproc.hpp>  
 #include <SFML/Graphics.hpp>
 #include <zbar.h>  
 #include "Textbox.h"
#include "Button.h"
 #include <iostream>  
 using namespace cv;  
 using namespace std;  
 using namespace zbar;  
 //g++ main.cpp /usr/local/include/ /usr/local/lib/ -lopencv_highgui.2.4.8 -lopencv_core.2.4.8  
 int main(int argc, char* argv[])  
 {  

  const int WINDOW_WIDTH = 880;
  const int WINDOW_HEIGHT = 820;

// -------------------------------------------------------
// -------------------------------------------------------
// -------------------------------------------------------
// -------------------------------------------------------
sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Student Verification System");
sf::Font font;
sf::Text text;
Button btn1("Scan Now", { 200, 50 }, 20, sf::Color::Blue, sf::Color::White);

sf::Image image, pfp;
        if (!(image.loadFromFile("nu-logo.jpg")) || !(pfp.loadFromFile("no_profile.jpg")))
                std::cout << "Cannot load image";   //Load Image
        
        sf::Texture texture, t2;
        texture.loadFromImage(image);  //Load Texture from image
        t2.loadFromImage(pfp);

        sf::Sprite sprite, sp2;
        sprite.setTexture(texture);  
        sp2.setTexture(t2);

if (!font.loadFromFile("arial.ttf"))
{
    cout << "Error loading font" << endl;
}

text.setFont(font); // font is a sf::Font
text.setString("Student Verification System");
text.setCharacterSize(35); // in pixels, not points!
text.setFillColor(sf::Color::Black);
text.setStyle(sf::Text::Bold | sf::Text::Underlined);
sprite.setPosition(WINDOW_WIDTH/6.f, 20.f);
sp2.setPosition(WINDOW_WIDTH/3.3, 300.f);
text.setPosition((WINDOW_WIDTH/4.f), 220.f);
btn1.setFont(font);
btn1.setPosition({ 350, 670 });

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (btn1.isMouseOver(window))
					      btn1.setBackColor(sf::Color::Cyan);
            else
                btn1.setBackColor(sf::Color::Blue);
        }

        window.clear(sf::Color::White);
        window.draw(text);
        window.draw(sprite);
        window.draw(sp2);
        btn1.drawTo(window);
        window.display();
    }
// -------------------------------------------------------
// -------------------------------------------------------
// -------------------------------------------------------
// -------------------------------------------------------


   VideoCapture cap(0); // open the video camera no. 0  
   // cap.set(CV_CAP_PROP_FRAME_WIDTH,800);  
   // cap.set(CV_CAP_PROP_FRAME_HEIGHT,640);  
   if (!cap.isOpened()) // if not success, exit program  
   {  
     cout << "Cannot open the video cam" << endl;  
     return -1;  
   }  
   ImageScanner scanner;   
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);   
   double dWidth = cap.get(CAP_PROP_FRAME_WIDTH); //get the width of frames of the video  
   double dHeight = cap.get(CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video  
   cout << "Frame size : " << dWidth << " x " << dHeight << endl;  
   namedWindow("QR Detection",WINDOW_AUTOSIZE); //create a window 
   while (1)  
   {  
     Mat frame;  
     bool bSuccess = cap.read(frame); // read a new frame from video  
      if (!bSuccess) //if not success, break loop  
     {  
        cout << "Cannot read a frame from video stream" << endl;  
        break;  
     }  
     Mat grey;  
     cvtColor(frame,grey,COLOR_BGRA2GRAY);  
     int width = frame.cols;   
     int height = frame.rows;   
     uchar *raw = (uchar *)grey.data;   
     // wrap image data   
     Image image(width, height, "Y800", raw, width * height);   
     // scan the image for barcodes   
     int n = scanner.scan(image);   
     // extract results   
     for(Image::SymbolIterator symbol = image.symbol_begin();   
     symbol != image.symbol_end();   
     ++symbol) {   
         vector<Point> vp;   
     // do something useful with results   
     cout << "decoded " << symbol->get_type_name()  << " symbol \"" << symbol->get_data() << '"' <<" "<< endl;
	int n = symbol->get_location_size();   
       for(int i=0;i<n;i++){   
         vp.push_back(Point(symbol->get_location_x(i),symbol->get_location_y(i)));   
       }   
       RotatedRect r = minAreaRect(vp);   
       Point2f pts[4];   
       r.points(pts);   
       for(int i=0;i<4;i++){   
         line(frame,pts[i],pts[(i+1)%4],Scalar(255,0,0),3);   
       }   
       //cout<<"Angle: "<<r.angle<<endl;   
     }   
     imshow("QR Detection", frame); //show the frame in "MyVideo" window  
     if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop  
     {  
       cout << "esc key is pressed by user" << endl;  
       break;   
     }  
   }  
   return 0;  
 }  
