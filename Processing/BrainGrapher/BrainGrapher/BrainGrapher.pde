// Main controller / model file for the the Processing Brain Grapher.

// See README.markdown for more info.
// See http://frontiernerds.com/brain-hack for a tutorial on getting started with the Arduino Brain Library and this Processing Brain Grapher.

// Latest source code is on https://github.com/kitschpatrol/Processing-Brain-Grapher
// Created by Eric Mika in Fall 2010, updates Spring 2012 and again in early 2014.

import processing.serial.*;
import controlP5.*;

ControlP5 controlP5;

Serial serial;

Channel[] channels = new Channel[15];
Monitor[] monitors = new Monitor[14];
Graph graph;
ConnectionLight connectionLight;

// Data directory (hardcoded to my home dir for now)
String data_directory = "/Users/jacob/Code/philometer/data/";
PrintWriter data_output;

int packetCount = 0;
int globalMax = 0;
String scaleMode;

void setup() {
  // Set up window
  size(1024, 768);
  frameRate(60);
  smooth();
  frame.setTitle("Processing Brain Grapher");  

  // Set up serial connection
  println("Find your Arduino in the list below, note its [index]:\n");

/*
  for (int i = 0; i < Serial.list().length; i++) {
    println("[" + i + "] " + Serial.list()[i]);
    String serial_string = Serial.list()[i];
    if (serial_string.matches("/dev/tty.usb*")) {
      print("Using Serial: " + Serial.list()[i]);
      serial = new Serial(this, Serial.list()[5], 9600);    
      serial.bufferUntil(10);
    }
  }
*/

  // Put the index found above here:
  serial = new Serial(this, Serial.list()[5], 9600);    
  serial.bufferUntil(10);

  // Set up the ControlP5 knobs and dials
  controlP5 = new ControlP5(this);
  controlP5.setColorLabel(color(0));    
  controlP5.setColorBackground(color(0));
  controlP5.disableShortcuts(); 
  controlP5.disableMouseWheel();
  controlP5.setMoveable(false);

  // Create the channel objects
  channels[0] = new Channel("Signal Quality", color(0), "");
  channels[1] = new Channel("Attention", color(100), "");
  channels[2] = new Channel("Meditation", color(50), "");
  channels[3] = new Channel("Delta", color(166,206,227), "Dreamless Sleep");
  channels[4] = new Channel("Theta", color(31,120,180), "Drowsy");
  channels[5] = new Channel("Low Alpha", color(51,160,44), "Relaxed");
  channels[6] = new Channel("High Alpha", color(251,154,153), "Relaxed");
  channels[7] = new Channel("Low Beta", color(227,26,28), "Alert");
  channels[8] = new Channel("High Beta", color(253,191,111), "Alert");
  channels[9] = new Channel("Low Gamma", color(255,127,0), "Multi-sensory processing");
  channels[10] = new Channel("High Gamma", color(202,178,214), "???");
  channels[11] = new Channel("Heart MS", color(106,61,154), "Heart MS");
  channels[12] = new Channel("Heart BPM", color(255,255,153), "Beats Per Minute");
  channels[13] = new Channel("GSR", color(178,223,138), "Galvanic Skin Response");
  channels[14] = new Channel("Temp", color(177,89,40), "Body Temperature");

  // Manual override for a couple of limits.
  channels[0].minValue = 0;
  channels[0].maxValue = 200;
  channels[1].minValue = 0;
  channels[1].maxValue = 100;
  channels[2].minValue = 0;
  channels[2].maxValue = 100;
  channels[0].allowGlobal = false;
  channels[1].allowGlobal = false;
  channels[2].allowGlobal = false;

  // Set up the monitors, skip the signal quality
  for (int i = 0; i < monitors.length; i++) {
    monitors[i] = new Monitor(channels[i + 1], i * (width / monitors.length), height / 2, width / monitors.length, height / 2);
  }

  monitors[monitors.length - 1].w += width % monitors.length;

  // Set up the graph
  graph = new Graph(0, 0, width, height / 2);

  // Set yup the connection light
  connectionLight = new ConnectionLight(width - 140, 10, 20);
  
  // Setup our output data file
  String data_file = data_directory + isoDateTime() + ".txt";
  println("Writing raw data to: " + data_file);
  data_output = createWriter(data_file);
}

void draw() {
  // Keep track of global maxima
  if (scaleMode == "Global" && (channels.length > 3)) {
    for (int i = 3; i < channels.length; i++) {
      if (channels[i].maxValue > globalMax) globalMax = channels[i].maxValue;
    }
  }

  // Clear the background
  background(255);

  // Update and draw the main graph
  graph.update();
  graph.draw();

  // Update and draw the connection light
  connectionLight.update();
  connectionLight.draw();

  // Update and draw the monitors
  for (int i = 0; i < monitors.length; i++) {
    monitors[i].update();
    monitors[i].draw();
  }
}

void serialEvent(Serial p) {
  try {
    // Split incoming packet on commas
    // See https://github.com/kitschpatrol/Arduino-Brain-Library/blob/master/README for information on the CSV packet format
  
    String incomingString = p.readString().trim();
    //print("Received string over serial: "); 
    //println(incomingString);  
  
    // Dump our string to the data file
    data_output.println(incomingString);
    data_output.flush();
    
    String[] incomingValues = split(incomingString, ',');
  
    // Verify that the packet looks legit
    if (incomingValues.length > 1) {
      packetCount++;
  
      // Wait till the third packet or so to start recording to avoid initialization garbage.
      if (packetCount > 3) {
  
        for (int i = 2; i < incomingValues.length; i++) {
        //for (int i = 2; i < 14; i++) {
          String stringValue = incomingValues[i].trim();
  
          int newValue = Integer.parseInt(stringValue);
  
          // Zero the EEG power values if we don't have a signal.
          // Can be useful to leave them in for development.
          //if ((Integer.parseInt(incomingValues[2]) == 200) && (i > 2)) {
          //  newValue = 0;
          //}
  
          channels[i - 2].addDataPoint(newValue);
        }
      }
    }
  } catch(Exception e) {
    e.printStackTrace();
  }
}


// Utilities

// Extend Processing's built-in map() function to support the Long datatype
long mapLong(long x, long in_min, long in_max, long out_min, long out_max) { 
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Extend Processing's built-in constrain() function to support the Long datatype
long constrainLong(long value, long min_value, long max_value) {
  if (value > max_value) return max_value;
  if (value < min_value) return min_value;
  return value;
}

String isoDateTime() {
  String dt = year() + "-";
  if (month() <= 9) dt += "0";
  dt += month() + "-";
  if (day() <= 9) dt += "0";
  dt += day() + "-T";
  if (hour() <= 9) dt += "0";
  dt += hour() + ":";
  if (minute() <= 9) dt += "0";
  dt += minute() + ":";
  if (second() <= 9) dt += "0";
  dt += second();
  return dt;
}

void stop() {
  // Shut down our data output file
  data_output.flush();
  data_output.close();
}

int convertValue(String str) {
  if (str.contains(".")) {
    float f = Float.valueOf(str).floatValue();
    return round(f);
  } else {
    return Integer.parseInt(str);
  }
}
