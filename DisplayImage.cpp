/*
Ctrl + P to Run program from task dependencies
*/

#include <opencv2/highgui/highgui.hpp>  
 #include <opencv2/imgproc/imgproc.hpp>  
 #include <SFML/Graphics.hpp>
 #include <string.h>
 #include <zbar.h>  
 #include "Textbox.h"
#include "Button.h"
 #include <iostream>  
 using namespace cv;  
 using namespace std;  
 using namespace zbar;  
 int main(int argc, char* argv[])  
 {  

  const int WINDOW_WIDTH = 880;
  const int WINDOW_HEIGHT = 820;
  bool notDetected = true;
  string student_id;

  VideoCapture cap(0);
   if (!cap.isOpened()) // if not success, exit program  
   {  
     cout << "Error opening webcam" << endl;  
     return -1;  
   }  
   ImageScanner scanner;   
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);   
   double dWidth = cap.get(CAP_PROP_FRAME_WIDTH); //get the width of frames of the video  
   double dHeight = cap.get(CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video  
   cout << "Frame size : " << dWidth << " x " << dHeight << endl;  
   namedWindow("QR Detection",WINDOW_AUTOSIZE); //create a window 

// -------------------------------------------------------
// -------------------------------------------------------
// -------------------------------------------------------
// -------------------------------------------------------
sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Student Verification System");
sf::Font font, font2;
sf::Text text, statusText, name, roll, reason;
Button btn1("Scan Now", { 200, 50 }, 20, sf::Color::Blue, sf::Color::White);

sf::Image image, pfp, allowed, notAllowed;
        if (!(image.loadFromFile("nu-logo.jpg")) || !(pfp.loadFromFile("no_profile.jpg")) || !(allowed.loadFromFile("allowed.jpg")) || !(notAllowed.loadFromFile("not_allowed.jpg")) )
                std::cout << "Cannot load image";   //Load Image
        
        sf::Texture texture, t2, al, nal;
        texture.loadFromImage(image);  //Load Texture from image
        t2.loadFromImage(pfp);
        al.loadFromImage(allowed);
        nal.loadFromImage(notAllowed);

        sf::Sprite sprite, sp2, spa, spn;
        sprite.setTexture(texture);  
        sp2.setTexture(t2);
        spa.setTexture(al);
        spn.setTexture(nal);

if (!font.loadFromFile("arial.ttf") || !(font2.loadFromFile("quicksand.ttf")))
{
    cout << "Error loading font" << endl;
}

text.setFont(font);
name.setFont(font2);
roll.setFont(font2);
reason.setFont(font2);
statusText.setFont(font);
text.setString("Student Verification System");
text.setCharacterSize(35); // in pixels
statusText.setCharacterSize(40);
roll.setCharacterSize(25);
name.setCharacterSize(35);
reason.setCharacterSize(25);
text.setFillColor(sf::Color::Black);
text.setStyle(sf::Text::Bold | sf::Text::Underlined);
roll.setStyle(sf::Text::Bold);
name.setStyle(sf::Text::Bold);
reason.setStyle(sf::Text::Bold);
statusText.setStyle(sf::Text::Bold | sf::Text::Underlined);
sprite.setPosition(WINDOW_WIDTH/6.f, 20.f);
sp2.setPosition(WINDOW_WIDTH/3.3, 300.f);
spa.setPosition(WINDOW_WIDTH/3.3, 300.f);
spn.setPosition(WINDOW_WIDTH/3.3, 300.f);
text.setPosition((WINDOW_WIDTH/4.f), 220.f);
statusText.setPosition(650.f, 670.f);
roll.setPosition(20.f, 670.f);
name.setPosition(20.f, 700.f);
reason.setPosition(20.f, 740.f);
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
            if (event.type == sf::Event::MouseButtonPressed) {
              if (btn1.isMouseOver(window)) {
                while (notDetected)  
   {  
    //window.setVisible(false);
    window.close();
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
     student_id = symbol->get_data();
     cout << "decoded " << symbol->get_type_name()  << " symbol \"" << student_id << '"' <<" "<< endl;
      notDetected = false;   
      //window.close();
     
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
     imshow("QR Detection", frame); //show the frame in "QR Detection" window  
     if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop  
     {  
       cout << "esc key is pressed by user" << endl;  
       destroyWindow("QR Detection");
       window.close();
       break;   
     }  
   }  
              }
            }
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

  cout << "STUDENT ID: " << student_id;
  destroyAllWindows();
  sf::RenderWindow window2(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Student Verification System");
  
  // if not allowed
  Button btn2("Valid Till: 22:30", { 200, 50 }, 20, sf::Color::Red, sf::Color::White);
  btn2.setFont(font);
  btn2.setPosition({ 335, 800 });


  statusText.setString("Not Allowed");
  statusText.setFillColor(sf::Color::Red);
  name.setString("Muhammad Abdullah Zafar");
  name.setFillColor(sf::Color::Black);
  roll.setString("22F-3777");
  roll.setFillColor(sf::Color::Black);
  reason.setString("Medicine");
  reason.setFillColor(sf::Color::Black);

  while(window2.isOpen()) {
    window2.clear(sf::Color::White);
  window2.draw(text);
  window2.draw(sprite);
  window2.draw(spn);
  window2.draw(statusText);
  window2.draw(name);
  window2.draw(roll);
  window2.draw(reason);
  btn2.drawTo(window2);
  window2.display();
  }
  
   return 0;  
 }  
