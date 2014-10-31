/* ----------------------------------------------------------------
   name:           Robot.cpp
   purpose:        implementation of the scenegraph construction
   version:	   SKELETON CODE
   TODO:           buildRobot
   author:         katrin lang
		   computer graphics
		   tu berlin
   ------------------------------------------------------------- */

#include "Robot.hpp"

// nodes of the scenegraph

// XXX: CURRENTLY ONLY TORSO. INSERT OTHER BODY PARTS HERE.
//      SEE EXAMPLE BINARY  cg1_ex1.exe (win32) and cg1_ex1_linux/cg1_ex1_mac.
//      EXPERIMENTATION IS ENCOURAGED. THE FINAL MODEL/SCENEGRAPH
//      NEED NOT BE IDENTICAL TO THE PROPOSED SOLUTION.

// XXX: EXTEND THE SCENEGRAPH i.e.
// Node *part1, *part2, *part3, etc...

// build the robot scenegraph
Node *buildRobot(){
  
  // torso attributes and initial position.
  // for parameters of constructor and their meanings,
  // see file node.h
  Node *torso= new Node(0, 0, 0,
			200, 300, 100,
			0, 0, 0,
			0, 0, 0);
  
  // XXX: implement more body/robot/whatever parts here

  Node *head= new Node(0, 235, 0,
			80, 80, 80,
			0, -60, 0,
			0, 0, 0);

  Node *larm= new Node(-165, 75, 0,
			40, 150, 40,
			40, 60, 0,
			0, 0, 0);

  Node *rarm= new Node(165, 75, 0,
			40, 150, 40,
			-40, 60, 0,
			0, 0, 0);

  Node *lforearm= new Node(0, -175, 0,
			40, 100, 40,
			0, 75, 0,
			0, 0, 0);

  Node *rforearm= new Node(0, -175, 0,
			40, 100, 40,
			0, 75, 0,
			0, 0, 0);

  Node *lleg= new Node(-50, -280, 0,
			80, 160, 80,
			0, 100, 0,
			0, 0, 0);

  Node *rleg= new Node(50, -280, 0,
			80, 160, 80,
			0, 100, 0,
			0, 0, 0);

  // finally, attach the robot/model parts
  // example: leftArm->setParent(torso), etc...
  // BUILD THE SCENEGRAPH

  head->setParent(torso);
  larm->setParent(torso);
  rarm->setParent(torso);
  lforearm->setParent(larm);
  rforearm->setParent(rarm);
  lleg->setParent(torso);
  rleg->setParent(torso);

  // return root node
  return torso;
}
