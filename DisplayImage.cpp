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
#include<string>
#include<chrono>
#include<thread>
#include"pstreams/pstream.h"
 #include <iostream>  
 #include <stdio.h>
 #include <jsoncpp/json/json.h>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>
#include <jsoncpp/json/value.h>
 using namespace cv;  
 using namespace std;  
 using namespace zbar;  
 using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds


string getValue(string strJson, string key) {

    Json::Value root;   
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( strJson.c_str(), root );     //parse process
    if ( !parsingSuccessful )
    {
        cout  << "Failed to parse"
               << reader.getFormattedErrorMessages();
        return 0;
    }

  return root.get(key, "A Default Value if not exists" ).asString();
}

string printRemainingTime(double created) {
  
  int min, sec;
  string res = "", minPrefix = "", secPrefix = "";

  milliseconds ms = duration_cast< milliseconds >(
    system_clock::now().time_since_epoch()
  );

  double currentTime = ms.count()/1000;
  double n = currentTime - created;

  const int seconds = n;
  min = 30 - (seconds/60);
  sec = 60 - (seconds%60);

        if (min < 0) {
            min = 0;
            sec = 0;
        }

  secPrefix = (sec < 10) ? "0" : "";
  minPrefix = (min < 10) ? "0" : "";

  res = "Valid Till: " + minPrefix + to_string(min) + ":" + secPrefix + to_string(sec);
  cout << "\nin function: res: " << res << endl;

  return res;
}

bool isTimeLeft(double created) {
  
  int min, sec;

  milliseconds ms = duration_cast< milliseconds >(
    system_clock::now().time_since_epoch()
  );

  double currentTime = ms.count()/1000;
  double n = currentTime - created;

  const int seconds = n;
  min = 30 - (seconds/60);

        if (min < 0) {
            return 0;
        }
  return 1;
}

 int main()  
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
text.setCharacterSize(35); // in pixe5ls
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
     cout << "symbol->get_data(): " << symbol->get_data();   
     student_id = symbol->get_data();
     cout << "decoded " << symbol->get_type_name()  << " symbol \"" << student_id << '"' <<" "<< endl;
     //sleep_for(seconds(1));
      notDetected = false;   
      
     
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
  
// run a process and create a streambuf that reads its stdout and stderr
 // char url[42] = "curl https://api-fast-flax.vercel.app/";
 string url = "curl https://api-fast-flax.vercel.app/";
  //url[38] = student_id[0]; url[39] = student_id[1]; url[40] = student_id[2]; url[41] = student_id[3];
  url = url + student_id;
  redi::ipstream proc(url, redi::pstreams::pstdout | redi::pstreams::pstderr);
  string line;
  // read child's stdout
  while (getline(proc.out(), line))
  {
    // line is text in json
    cout << "stdout: " << line << '\n';
  }
  string fName = getValue(line, "name");
  string fReason = getValue(line, "reason");
  double created = stod(getValue(line, "created"));
  string remTime = printRemainingTime(created);
  bool isAllowed = isTimeLeft(created);
  
  // if reading stdout stopped at EOF then reset the state:
  if (proc.eof() && proc.fail())
    proc.clear();
  // read child's stderr
  while (getline(proc.err(), line))
    cout << "stderr: " << line << '\n';

  // if not allowed
  if (!isAllowed) {
    statusText.setString("Not Allowed");
    statusText.setFillColor(sf::Color::Red);
  } else {
    statusText.setString("Allowed");
    statusText.setFillColor(sf::Color::Green);
  }
  Button btn2(remTime, { 200, 50 }, 20, sf::Color::Red, sf::Color::White);
  btn2.setFont(font);
  btn2.setPosition({ 335, 800 });
  name.setString(fName);
  name.setFillColor(sf::Color::Black);
  //char ro[8] = "22F-";
  //ro[4] = student_id[0];ro[5] = student_id[1]; ro[6] = student_id[2]; ro[7] = student_id[3];
  string ro = "22F-" + student_id;
  //cout << "\nro: " << ro;
  roll.setString(ro);

  roll.setFillColor(sf::Color::Black);
  reason.setString(fReason);
  reason.setFillColor(sf::Color::Black);

 // string txt = printRemainingTime(stod(getValue(line, "created")));
  while(window2.isOpen()) {
    window2.clear(sf::Color::White);
  window2.draw(text);
  window2.draw(sprite);
  if (isAllowed)
    window2.draw(spa);
  else
    window2.draw(spn);
  window2.draw(statusText);
  window2.draw(name);
  window2.draw(roll);
  window2.draw(reason);

  // update validity in real-time
  // this gives error
  //btn2.setText("Valid Till: " + );
  //remTime = printRemainingTime(stod(getValue(line, "created")));
  //btn2.setText(remTime);
  btn2.drawTo(window2);
  window2.display();
  }
  
   return 0;  
 }  
