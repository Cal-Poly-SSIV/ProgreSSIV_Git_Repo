/* CAN Firmware Functions made for Teensy 3.6 
	by the uLaren Senior Project Team    */

/*********************************/
/* current functionality works but re-initialization does not work*/
/*********************************/

#include "flexCAN.h"
#include "ProgreSSIV_MC_Driver.h"
#include <string.h>

#define WAIT_FOR_RESPONSE_TIME_SLOW_US 100
#define WAIT_FOR_RESPONSE_TIME_FAST_US 50


extern FlexCAN CANbus;

int reset_nodes()
{
  CAN_message_t msg;
  int ret = 0;

  //reset nodes NMT command
  msg.id = 0;
  msg.ext = 0;
  msg.len = 2;
  msg.buf[0] = 0x81;
  msg.buf[1] = 0;

  msg.timeout = 1;//Milliseconds before giving up on message
  if (CANbus.write(msg) == 0)
  {
    ret = ERROR_CAN_WRITE;
    if (PRINT)
    {
        Serial.println();
        Serial.println("error CAN write during reset_nodes() function call");
        Serial.println();
        delay(500);
    }
    
  }
  else{
    ret = NO_ERROR;
    if (PRINT)
    { 
        Serial.println();
        Serial.println("Successful CAN write during reset_nodes() function call");
        Serial.println();
        delay(500);
    }
  }
  if(ret == NO_ERROR){//If the CAN write was successful, attempt to read the boot up messages from the CAN bus 
    for(uint8_t index = 0; index<4 ; index++){
      
      msg.timeout = 1000;//Milliseconds before giving up on reading a CAN message
      if (CANbus.read(msg) != 0){//If the read message timeout wasnt reached and a message was available to read
        if (PRINT)
        {
          Serial.println();
          Serial.println("Message read during reset_nodes function call:");
          Serial.println();
          print_CAN_message(msg);
        }
      }
  
      else{
        if(PRINT)
        {
          Serial.println();
          Serial.println("A read message timeout has occured during the reset_nodes function call.");
          Serial.println();
        }
      }
    }
  }


  return ret; //May want to change what each of these functions returns
}

int reset_communications()
{
  CAN_message_t msg;
  int ret = 0;

  //reset communication NMT command
  msg.id = 0;
  msg.ext = 0;
  msg.len = 2;
  msg.buf[0] = 0x82;
  msg.buf[1] = 0;

  msg.timeout = 1;//Milliseconds before giving up on broadcastimng CAN message
  if (CANbus.write(msg) == 0)
  {
    ret = ERROR_CAN_WRITE;
    if (PRINT)
    {
        Serial.println();
        Serial.println("error CAN write during reset_communication() function call");
        Serial.println();
        delay(500);
    }
    
  }
  else{
    ret = NO_ERROR;
    if (PRINT)
    {
        Serial.println();
        Serial.println("successful CAN write during reset_communication() function call");
        Serial.println();
        delay(500);
    }
  }

  if(ret == NO_ERROR){//If the CAN write was successful, attempt to read the boot up messages from the CAN bus 
    
    for(uint8_t index = 0; index<4 ; index++){
      
      msg.timeout = 1000;//Milliseconds before giving up on reading a CAN message
      if (CANbus.read(msg) != 0){//If the read message timeout wasnt reached and a message was available to read
        if (PRINT)
        {
          Serial.println();
          Serial.println("Message read during reset_communications function call:");
          Serial.println();
          print_CAN_message(msg);
        }
      }
  
      else{
        if(PRINT)
        {
          Serial.println();
          Serial.println("A read message timeout has occured during the reset_communications function call.");
          Serial.println();
        }
      }
    }
  }
  
  return ret; //May want to change what each of these functions returns
}


int start_remote_nodes()
{
	CAN_message_t msg;
  int ret = 0;

	//"Start Remote Node CAN Message"
	msg.id = 0;//CAN_id for NMT or network management
	msg.ext = 0;
	msg.len = 2;
	msg.timeout = 0; //Tells how long the system should wait for a response to this CAN message? LOOK AT FLEXCAN README FOR ANSWER TO THIS
	msg.buf[0] = 0x01; //Command Specifier for "start_remote_node"
	msg.buf[1] = 0;//when set to zero, all slaves will be started
 

	if (CANbus.write(msg) == 0)
 {
    ret = ERROR_CAN_WRITE;
 }
  //print_CAN_message(msg);

  delayMicroseconds(WAIT_FOR_RESPONSE_TIME_SLOW_US);
  if (CANbus.read(msg) != 0)
  {
    if (PRINT)
    {
      Serial.println("Received Start Node Confirmation");
    }
    
  }
  else
  {
    if (PRINT)
    {
      Serial.println("DID NOT Receive Start Node Confirmation");
    }
    
  }

  return ret;

}

int resetFault (int node_id) 
{
    CAN_message_t msg;
  int ret = 0;

  //make sure node_id is valid!
  if (node_id > 0 && node_id < 127)
  {
    
  }
  else 
  {
    if (PRINT)
    {
      Serial.print("***** I received a bad node_id of: ");
      Serial.println(node_id);
    }
    
    exit(0);
  }

  //tell MC's to reset faults
  msg.id = 0x600 + node_id;
  msg.ext = 0;
  msg.len = 6;
  msg.timeout = 0;
  //
  msg.buf[0] = 0x2B; 
  msg.buf[2] = 0x60; 
  msg.buf[1] = 0x40;
  msg.buf[3] = 0x00;
  //
  msg.buf[5] = 0x00;
  msg.buf[4] = 0b10000000;
  msg.buf[6] = 0;
  msg.buf[7] = 0;

  if (CANbus.write(msg) == 0)
  {
    ret = ERROR_CAN_WRITE;
    if (PRINT)
    {
      Serial.println("ERROR: CAN Write.");
      Serial.print("Node id: ");
      Serial.println(node_id);
    }
    
  }
  else {
//    //print_CAN_message(msg);
//    delayMicroseconds(WAIT_FOR_RESPONSE_TIME_SLOW_US);
//    if (CANbus.read(msg) != 0)
//    {
//      if (PRINT)
//      {
//        Serial.println("Received Shutdown Confirmation");
//      }
//      
//    }
//    else
//    {
//      if (PRINT)
//      {
//        Serial.println("DID NOT Receive Shutdown Confirmation");
//      }
//      
//    }
  }
    //tell MC to reset communication
  msg.id = 0x00;
  msg.ext = 0;
  msg.len = 2;
  msg.timeout = 0;
  //
  msg.buf[0] = 0x01;  //68
  msg.buf[2] = 0x00 + node_id;  //60
  msg.buf[1] = 0x00;
  msg.buf[3] = 0x00;
  //
  msg.buf[5] = 0x00;
  msg.buf[4] = 0x00;
  msg.buf[6] = 0;
  msg.buf[7] = 0;

  if (CANbus.write(msg) == 0)
  {
    ret = ERROR_CAN_WRITE;
    if (PRINT)
    {
      Serial.println("ERROR: CAN Write.");
      Serial.print("Node id: ");
      Serial.println(node_id);
    }
    
  }
  else {
//    //print_CAN_message(msg);
//    delayMicroseconds(WAIT_FOR_RESPONSE_TIME_SLOW_US);
//    if (CANbus.read(msg) != 0)
//    {
//      if (PRINT)
//      {
//        Serial.println("Received Shutdown Confirmation");
//      }
//      
//    }
//    else
//    {
//      if (PRINT)
//      {
//        Serial.println("DID NOT Receive Shutdown Confirmation");
//      }
//      
//    }
  }
}

int initialize_MC_Profile_Vel_Mode(int node_id)
{
	CAN_message_t msg;
  int ret = 0;

  //make sure node_id is valid!
  if (node_id > 0 && node_id < 127)
  {
    
  }
  else 
  {
    if (PRINT)
    {
      Serial.print("***** I received a bad node_id of: ");
      Serial.println(node_id);
    }
    
    exit(0);
  }

	//tell MC's to go to shutdown state
	msg.id = 0x600 + node_id;
	msg.ext = 0;
	msg.len = 6;
	msg.timeout = 0;
	//
	msg.buf[0] = 0x2B;  
	msg.buf[2] = 0x60;  
	msg.buf[1] = 0x40;
	msg.buf[3] = 0x00;
	//
	msg.buf[5] = 0x00;
	msg.buf[4] = 0b00000110;
  msg.buf[6] = 0;
  msg.buf[7] = 0;

	if (CANbus.write(msg) == 0)
  {
    ret = ERROR_CAN_WRITE;
    if (PRINT)
    {
      Serial.println("ERROR: CAN Write.");
      Serial.print("Node id: ");
      Serial.println(node_id);
    }
    
  }
  else {
//    //print_CAN_message(msg);
//    delayMicroseconds(WAIT_FOR_RESPONSE_TIME_SLOW_US);
//    if (CANbus.read(msg) != 0)
//    {
//      if (PRINT)
//      {
//        Serial.println("Received Shutdown Confirmation");
//      }
//      
//    }
//    else
//    {
//      if (PRINT)
//      {
//        Serial.println("DID NOT Receive Shutdown Confirmation");
//      }
//      
//    }
  }
  
	//Set MC's to profile velocity mode
  msg.id = 0x600 + node_id;
	msg.len = 5;
  msg.timeout = 0;
	msg.buf[0] = 0x2F;
	msg.buf[2] = 0x60;
	msg.buf[1] = 0x60;
	msg.buf[3] = 0;
	msg.buf[4] = PROFILE_VELOCITY_MODE;

	if (CANbus.write(msg) == 0)
  {
    ret = ERROR_CAN_WRITE;
    if (PRINT)
    {
      Serial.println("ERROR: CAN Write.");
      Serial.print("Node id: ");
      Serial.println(node_id);
    }
    
  }
  else {
    //print_CAN_message(msg);
    delayMicroseconds(WAIT_FOR_RESPONSE_TIME_SLOW_US);
    if (CANbus.read(msg) != 0)
    {
      if (PRINT)
      {
        Serial.println("Received Profile Velocity Confirmation");
      }
      
    }
    else
    {
      if (PRINT)
      {
        Serial.println("DID NOT Receive Profile Velocity Confirmation");
      }
      
    }
  }
  
	//initialize MCs motion profile type : linear
  msg.id = 0x600 + node_id;
	msg.len = 6;
	msg.buf[0] = 0x2B;
	msg.buf[2] = 0x60;
	msg.buf[1] = 0x86;
	msg.buf[3] = 0;
	msg.buf[4] = 0;
	msg.buf[5] = 0;

	if (CANbus.write(msg) == 0)
  {
    ret = ERROR_CAN_WRITE;
    if (PRINT)
    {
      Serial.println("ERROR: CAN Write.");
      Serial.print("Node id: ");
      Serial.println(node_id);
    }
    
  }
  else {
    //print_CAN_message(msg);
    delayMicroseconds(WAIT_FOR_RESPONSE_TIME_SLOW_US);
    if (CANbus.read(msg) != 0)
    {
      if (PRINT)
      {
        Serial.println("Received Motion Profile Confirmation");
        //print_CAN_message(msg);
      }
      
    }
    else
    {
      if (PRINT)
      {
        Serial.println("DID NOT Receive Motion Profile Confirmation");
      }
      
    }
  }
  
	/*write max profile velocity
	msg.len = 8;
	msg.buf[0] = 0x68;
	msg.buf[1] = 0x60;
	msg.buf[2] = 0x7F;
	msg.buf[3] = 0;
	//velocity goes in here. refer to 6.2.81 & 2-15
	msg.buf[4] = 0;
	msg.buf[5] = 0;
	msg.buf[6] = 0;
	msg.buf[7] = 0;

	write_message_to_all_MCs(msg);
	*/
  
  //tell MC's to go to switch-on state
  msg.id = 0x600 + node_id;
  msg.ext = 0;
  msg.len = 6;
  msg.timeout = 0;
  //
  msg.buf[0] = 0x2B;
  msg.buf[2] = 0x60;
  msg.buf[1] = 0x40;
  msg.buf[3] = 0x00;
  //
  msg.buf[5] = 0x00;
  msg.buf[4] = 0b00000111;

  if (CANbus.write(msg) == 0)
  {
    ret = ERROR_CAN_WRITE;
    if (PRINT)
    {
      Serial.println("ERROR: CAN Write.");
      Serial.print("Node id: ");
      Serial.println(node_id);
    }
    
  }
  else {
    //print_CAN_message(msg);
    delayMicroseconds(WAIT_FOR_RESPONSE_TIME_SLOW_US);
    if (CANbus.read(msg) != 0)
    {
      if (PRINT)
      {
        Serial.println("Received Switch ON Confirmation");
      }
      
    }
    else
    {
      if (PRINT)
      {
        Serial.println("DID NOT Receive Switch ON Confirmation");
      }
      
    }
  }
  return ret;
}

int initialize_MC_Torque_Mode(int node_id)
{
  CAN_message_t msg;
  int ret = 0;

  //make sure node_id is valid!
  if (node_id > 0 && node_id < 127)
  {
    
  }
  else 
  {
    if (PRINT)
    {
      Serial.print("***** I received a bad node_id of: ");
      Serial.println(node_id);
    }
    
    exit(0);
  }

  //tell MC's to go to shutdown state
  msg.id = 0x600 + node_id;
  msg.ext = 0;
  msg.len = 6;
  msg.timeout = 0;
  //
  msg.buf[0] = 0x2B;  
  msg.buf[2] = 0x60;  
  msg.buf[1] = 0x40;
  msg.buf[3] = 0x00;
  //
  msg.buf[5] = 0x00;
  msg.buf[4] = 0b00000110;
//  msg.buf[6] = 0;
//  msg.buf[7] = 0;

  if (CANbus.write(msg) == 0)
  {
    ret = ERROR_CAN_WRITE;
    if (PRINT)
    {
      Serial.println("ERROR: CAN Write.");
      Serial.print("Node id: ");
      Serial.println(node_id);
    }
    
  }
  else {
//    //print_CAN_message(msg);
//    delayMicroseconds(WAIT_FOR_RESPONSE_TIME_SLOW_US);
//    if (CANbus.read(msg) != 0)
//    {
//      if (PRINT)
//      {
//        Serial.println("Received Shutdown Confirmation");
//      }
//      
//    }
//    else
//    {
//      if (PRINT)
//      {
//        Serial.println("DID NOT Receive Shutdown Confirmation");
//      }
//      
//    }
  }
  
  //initialize MC's to torque mode
  msg.id = 0x600 + node_id;
  msg.len = 5;
  msg.timeout = 0;
  msg.buf[0] = 0x2F;
  msg.buf[2] = 0x60;
  msg.buf[1] = 0x60;
  msg.buf[3] = 0;
  msg.buf[4] = TORQUE_MODE;

  if (CANbus.write(msg) == 0)
  {
    ret = ERROR_CAN_WRITE;
    if (PRINT)
    {
      Serial.println("ERROR: CAN Write.");
      Serial.print("Node id: ");
      Serial.println(node_id);
    }
    
  }
  else {
    //print_CAN_message(msg);
    delayMicroseconds(WAIT_FOR_RESPONSE_TIME_SLOW_US);
    if (CANbus.read(msg) != 0)
    {
      if (PRINT)
      {
        Serial.println("Received Torque Mode Confirmation");
      }
      
    }
    else
    {
      if (PRINT)
      {
        Serial.println("DID NOT Receive Torque Mode Confirmation");
      }
      
    }
  }
  
  //tell MC's to go to switch-on state
  msg.id = 0x600 + node_id;
  msg.ext = 0;
  msg.len = 6;
  msg.timeout = 0;
  //Command Specifier for Write Dictionary Object of 2 bytes
  msg.buf[0] = 0x2B;
  //Control word Object dictionary index
  msg.buf[2] = 0x60;
  msg.buf[1] = 0x40;
  //Control word Object dictionary sub-index
  msg.buf[3] = 0x00;
  //Control word bytes
  msg.buf[5] = 0x00;//Controlword high byte
  msg.buf[4] = 0b00000111;//Controlword low byte

  if (CANbus.write(msg) == 0)
  {
    ret = ERROR_CAN_WRITE;
    if (PRINT)
    {
      Serial.println("ERROR: CAN Write.");
      Serial.print("Node id: ");
      Serial.println(node_id);
    }
    
  }
  else {
    //print_CAN_message(msg);
    delayMicroseconds(WAIT_FOR_RESPONSE_TIME_SLOW_US);
    if (CANbus.read(msg) != 0)
    {
      if (PRINT)
      {
        Serial.println("Received Switch ON Confirmation");
      }
      
    }
    else
    {
      if (PRINT)
      {
        Serial.println("DID NOT Receive Switch ON Confirmation");
      }
      
    }
  }
  return ret;
}


int arm_MC(int node_id)
{
  CAN_message_t msg;
  int ret = 0;

  //make sure node_id is valid!
  if (node_id > 0 && node_id < 127)
  {
    
  }
  else 
  {
    if (PRINT)
    {
      Serial.print("***** I received a bad node_id of: ");
    Serial.println(node_id);
    }
    
    exit(0);
  }

  //tell MC's to go to operation enabled state
  msg.id = 0x600 + node_id;
  msg.ext = 0;
  msg.len = 6;
  msg.timeout = 0;
  //
  msg.buf[0] = 0x2B;
  msg.buf[2] = 0x60;
  msg.buf[1] = 0x40;
  msg.buf[3] = 0x00;
  //
  msg.buf[5] = 0x01;//Controlword high byte
  msg.buf[4] = 0b00001111;//Controlword low byte.

  if (CANbus.write(msg) == 0)
  {
    ret = ERROR_CAN_WRITE;
  }
  /*else {
    //print_CAN_message(msg);
    delayMicroseconds(WAIT_FOR_RESPONSE_TIME_SLOW_US);
    if (CANbus.read(msg) != 0)
    {
      Serial.println("Received Operation Enabled Confirmation");
      //print_CAN_message(msg);
    }
    else
    {
      Serial.println("DID NOT Receive Operation Enabled Confirmation");
    }
  }*/
  return ret;
}


//int write_throttle_to_MC(int node_id, int throttle)
//{
//  CAN_message_t msg;
//  int ret = 0;
//    
//  //make sure node_id is valid!
//  if (node_id > 0 && node_id < 127)
//  {
//    
//  }
//  else 
//  {
//    if (PRINT)
//    {
//      Serial.print("***** I received a bad node_id of: ");
//      Serial.println(node_id);
//    }
//    
//    exit(0);
//  }
//
//  //make sure throttle does not exceed limits
//  if (throttle > 500) 
//  {
//    throttle = 500;
//  }
//  else if (throttle < -500)
//  {
//    throttle = -500;
//  }
//  else if (throttle < 10 && throttle > -10)
//  {
//    throttle = 0;
//  }
//
//  //write to Target Velocity
//  msg.id = 0x600 + node_id;
//  msg.ext = 0;
//  msg.len = 8;
//  msg.timeout = 0;
//  msg.buf[0] = 0x23;
//  msg.buf[2] = 0x60;
//  msg.buf[1] = 0xFF;
//  msg.buf[3] = 0;
//  //
//  memcpy(&(msg.buf[4]), (void *)(&throttle), 1);
//  memcpy(&(msg.buf[5]), ((char *)(&throttle) + 1), 1);
//  memcpy(&(msg.buf[6]), ((char *)(&throttle) + 2), 1);
//  memcpy(&(msg.buf[7]), ((char *)(&throttle) + 3), 1);
//  
//  //write message and receive
//  if (CANbus.write(msg) == 0)
//  {
//    if (PRINT)
//    {
//      Serial.println("error writing CAN message");
//    }
//    
//    ret = ERROR_CAN_WRITE;
//  }
//  //it will stop responding if a can write occurs but i want to test this out
//  else 
//  {
//    if (PRINT)
//      {
//        //Serial.print("Awaiting throttle write confirmation... ");
//      }
//    while (CANbus.read(msg) == 0)
//    {
//      //wait for handshake
//      
//    }
//
//    if (PRINT)
//    {
//      //Serial.println("Received confirmation");
//    }
//  }
//  return ret;
//}
//
////int initiate_target_velocity (int node_id)
////{
////  CAN_message_t msg;
////  int ret = 0;
////  
////  //initiate target velocity
////  //tell MC's to go to operation enabled state w/target velocity enabled
////  msg.id = 0x600 + node_id;
////  msg.ext = 0;
////  msg.len = 6;
////  msg.timeout = 0;
////  //
////  msg.buf[0] = 0x2B;
////  msg.buf[2] = 0x60;
////  msg.buf[1] = 0x40;
////  msg.buf[3] = 0x00;
////  //
////  msg.buf[5] = 0b00000000;
////  msg.buf[4] = 0b00001111;
////  msg.buf[6] = 0;
////  msg.buf[7] = 0;
////
////  //write message
////  if (CANbus.write(msg) == 0)
////  {
////    ret = ERROR_CAN_WRITE;
////  }
////  else 
////  {
////  //it will stop responding but i want to test
////    //print_CAN_message(msg);
////    delayMicroseconds(WAIT_FOR_RESPONSE_TIME_FAST_US);
////    if (PRINT)
////    {
////      //Serial.print("Awaiting initiate target velocity confirmation... ");
////    }
////    while (CANbus.read(msg) == 0)
////    {
////      //wait for handshake
////    }
////
////    if (PRINT)
////    {
////      //Serial.println("Received confirmation");
////    }
////  }
////
////  return ret;
////}

int write_velocity_and_enable_op(int node_id, int throttle)
{
  CAN_message_t msg;
  int ret = 0;
  int error = 0;

  //make sure node_id is valid!
  if (node_id > 0 && node_id < 127)
  {
    
  }
  else 
  {
    if (PRINT)
    {
      Serial.print("***** I received a bad node_id of: ");
      Serial.println(node_id);
    }
    
    exit(0);
  }

//  
//  //make sure throttle does not exceed limits
//  if (throttle > (500 * SCALE_FACTOR + 200))  
//  {
//    throttle = 500 * SCALE_FACTOR;
//  }
//  else if (throttle < (-500 * SCALE_FACTOR - 200))
//  {
//    throttle = -500 * SCALE_FACTOR;
//  }
//
//  //initiate throttle dead zone
//  if (throttle < (10 * SCALE_FACTOR) && throttle > (-10 * SCALE_FACTOR))
//  {
//    throttle = 0;
//  }
  
  //RxPDO 4, setting the controlword value and updating the target velocity
  msg.id = 0x500 + node_id; //COB-id of RxPDO 4
  msg.ext = 0;
  msg.len = 6;
  msg.timeout = 0;
  //Control Word bytes
  msg.buf[0] = 0x0F;//Device control command for "Enable Operation" I BELIEVE
  msg.buf[1] = 0x00;//
  //Target velocity bytes
  memcpy(&(msg.buf[2]), (void *)(&throttle), 1);
  memcpy(&(msg.buf[3]), ((char *)(&throttle) + 1), 1);
  memcpy(&(msg.buf[4]), ((char *)(&throttle) + 2), 1);
  memcpy(&(msg.buf[5]), ((char *)(&throttle) + 3), 1);

  if (PRINT)
  {
    print_CAN_message(msg);
  }

  ret = CANbus.write(msg);
  if (ret == 1)
  {
    error = NO_ERROR;
  }
  else {
    error = ERROR_CAN_WRITE;
  }

  return 0;
}

int write_torque_and_enable_op(int node_id, int throttle)
{
  CAN_message_t msg;
  int ret = 0;
  int error = 0;

  //make sure throttle does not exceed limits
  if (throttle > (500 * SCALE_FACTOR + 200))  
  {
    throttle = 500 * SCALE_FACTOR;
  }
  else if (throttle < (-500 * SCALE_FACTOR - 200))
  {
    throttle = -500 * SCALE_FACTOR;
  }

  //initiate throttle dead zone
  if (throttle < (10 * SCALE_FACTOR) && throttle > (-10 * SCALE_FACTOR))
  {
    throttle = 0;
  }
  
  //RxPDO 3, setting the controlword value and updating the target torque
  msg.id = 0x400 + node_id; //COB-id of RxPDO 3 for this specific node
  msg.ext = 0;
  msg.len = 6;
  msg.timeout = 0;
  //Control Word bytes
  msg.buf[0] = 0x0F;
  msg.buf[1] = 0x00;
  //Target torque bytes
  memcpy(&(msg.buf[2]), (void *)(&throttle), 1);
  memcpy(&(msg.buf[3]), ((char *)(&throttle) + 1), 1);
  memcpy(&(msg.buf[4]), ((char *)(&throttle) + 2), 1);
  memcpy(&(msg.buf[5]), ((char *)(&throttle) + 3), 1);

  if (PRINT)
  {
    //print_CAN_message(msg);
  }

  ret = CANbus.write(msg);
  if (ret == 1)
  {
    error = NO_ERROR;
  }
  else {
    error = ERROR_CAN_WRITE;
  }

  return 0;
}

void print_CAN_message(CAN_message_t msg)
{
  if (PRINT)
  {
    Serial.print("ID: ");
    Serial.println(msg.id, HEX);
    Serial.print("Length: ");
    Serial.println(msg.len);
    Serial.print("Byte[0]: ");
    Serial.println(msg.buf[0], HEX);
    Serial.print("Byte[1]: ");
    Serial.println(msg.buf[1], HEX);
    Serial.print("Byte[2]: ");
    Serial.println(msg.buf[2], HEX);
    Serial.print("Byte[3]: ");
    Serial.println(msg.buf[3], HEX);
    Serial.print("Byte[4]: ");
    Serial.println(msg.buf[4], HEX);
    Serial.print("Byte[5]: ");
    Serial.println(msg.buf[5], HEX);
    Serial.print("Byte[6]: ");
    Serial.println(msg.buf[6], HEX);
    Serial.print("Byte[7]: ");
    Serial.println(msg.buf[7], HEX);
    Serial.println();
  }
  
}

int send_statusword_request(int node_id)
{
  CAN_message_t msg;
  int ret = 0;
  
  msg.id = 0x600 + node_id;
  msg.ext = 0;
  msg.len = 8;//Changed this to 8 so that the command specifier, index, subindex, and four bytes of junk data are sent (buf 0 - 7)
  msg.timeout = 0;
  //
  msg.buf[0] = 0x40;
  msg.buf[2] = 0x60;
  msg.buf[1] = 0x41;
  msg.buf[3] = 0x00;
  //
  msg.buf[4] = 0x00;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  msg.buf[7] = 0;

  if (CANbus.write(msg) == 0)
  {
    if (PRINT)
    {
      Serial.println("error writing CAN message");
    }
    
    exit(0);
  }
  
  print_CAN_message(msg);
  delayMicroseconds(WAIT_FOR_RESPONSE_TIME_FAST_US);
  /*if (CANbus.read(msg) != 0)
  {
    if (PRINT)
    {
      Serial.println("Received Statusword");
    }
    
    print_CAN_message(msg);
    ret = 1;
  }
  else
  {
    if (PRINT)
    {
      Serial.println("DID NOT Receive Statusword");
    }
    
    ret = 0;
  }*/

  return ret;
}

void check_available_msg()
{
  CAN_message_t msg;
  
  if (CANbus.available())
  {
    if (PRINT)
    {
      Serial.println("I have something to receive");
    }
    
    if (CANbus.read(msg) != 0)
    {
      if (PRINT)
      {
        Serial.println("I read a message!");
      }
      
      print_CAN_message(msg);
    }
  }
  else
  {
    if (PRINT)
    {
      Serial.println("nothing");
    }
    
  }
}



void process_available_msgs()
{
  CAN_message_t msg;
  
  //delayMicroseconds(WAIT_FOR_RESPONSE_TIME_SLOW_US);
  while (CANbus.available()) 
  {

    if (CANbus.read(msg) != 0)
    {
      print_CAN_message(msg);
    }
    else
    {
      if (PRINT)
      {
        Serial.println("***OOPS the CAN read went wrong!***");
      }
      
    }
  }
}

int stop_remote_node(int node_id)
{
  CAN_message_t msg;
  int ret = 0;

  //reset node coms
  msg.id = 0;
  msg.ext = 0;
  msg.len = 2;
  msg.timeout = 0;
  msg.buf[0] = 0x02;
  msg.buf[1] = 0;
  msg.buf[2] = 0;
  msg.buf[3] = 0;
  msg.buf[4] = 0;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  msg.buf[7] = 0;
  
  ret = CANbus.write(msg);
  //print_CAN_message(msg);
  delayMicroseconds(WAIT_FOR_RESPONSE_TIME_SLOW_US);
  if (CANbus.read(msg) != 0)
  {
    if (PRINT)
    {
      Serial.println("Received Stop Node Confirmation");
    }
    
  }
  else
  {
    if (PRINT)
    {
      Serial.println("DID NOT Receive Stop Node Confirmation");
    }
    
  }

  return ret;
}

int diagnose_controller_message(CAN_message_t msg)
{ 
    int message_status = 0;

    if (msg.id > 580 && msg.id < 680)
    {
       //this is a confirmation message
       if (msg.id == 581)
       {
         if (PRINT)
         {
           Serial.print("Node 1:");
         }
          
       }
    }
    //evan actually do this sometime if it will help
  return message_status;
}

int link_node(int node_id)
{
  CAN_message_t msg;
  int ret = 0;
  int armed = 0;
  int error = 0;
    
  //make sure node_id is valid!
  if (node_id > 0 && node_id < 127)
  {
    
  }
  else 
  {
    if (PRINT)
    {
      Serial.print("***** I received a bad node_id of: ");
      Serial.println(node_id);
    }
    
    exit(0);
  }
  
  //try to clear out can read buffer (is there a read buffer?)
  delay(200);
  while (CANbus.available())
  {
    CANbus.read(msg);
  }
  /***********************************************/
  //arm
  if (PRINT)
  {
    Serial.println("*****ARMING****");
    Serial.print("On node: ");
    Serial.println(node_id);
  }
      
  arm_MC(node_id);
  delay(1000);
  
  send_statusword_request(node_id);
  delay(200);
  
  while (CANbus.available())
  {
    CANbus.read(msg);
    if (PRINT)
    {
      print_CAN_message(msg);
    }
    
    //previously searched for a 1A0 node id but this isn't always the node id sent when the controller gets armed
    if (msg.buf[4] == 55 && msg.buf[5] == 22)  //37 and 16 in hex
    {
      //we're armed. lets go
      armed = 1;
    }
    
  }

  //attempt to rearm node
  while (armed == 0)
  {
    if (PRINT)
    {
      Serial.println("*****ATTEMPTING A RE-ARM****");
      Serial.print("On node: ");
      Serial.println(node_id);
    }
    

    armed = rearm_MC(node_id);
  }

  
  //write
  if (PRINT)
  {
    Serial.println("*****WRITING Torque and INITIATING****");
  }
  
  write_torque_and_enable_op(node_id,0);
  delay(50);
  while (CANbus.available())
  {
    CANbus.read(msg);
    //print_CAN_message(msg);
  }
  
  /*
  //arm
  Serial.println("*****ARMING****");
  arm_MC(NODE_1);
  delay(2000);
  while (CANbus.available())
  {
    CANbus.read(msg);
    print_CAN_message(msg);
  }
  //write
  Serial.println("*****WRITING VELOCITY****");
  write_throttle_to_MC(NODE_1,0);
  delay(1000);
  while (CANbus.available())
  {
    CANbus.read(msg);
    print_CAN_message(msg);
  }
  
  //initiate
  Serial.println("*****INITIATING****");
  initiate_target_velocity(NODE_1);
  delay(1000);
  while (CANbus.available())
  {
    CANbus.read(msg);
    print_CAN_message(msg);
  }
  //stall
  while (1)
  {
    if (CANbus.available())
    {
      CANbus.read(msg);
      print_CAN_message(msg);
    }
    //stall here so i can figure out what messages i should be expecting
  }*/

  return ret;
}

int rearm_MC(int node_id)
{
  CAN_message_t msg;
  int ret = 0;
  int error = 0;

  /*
  ret = initialize_MC_Profile_Vel_Mode(node_id);
  if (ret > 0)
  {
    error = ret;
  }

  ret = arm_MC(node_id);
  if (ret > 0)
  {
    error = ret;
  }*/
  
  delay(200);
  resetFault(node_id);
  
  delay(200);
  initialize_MC_Torque_Mode(node_id);
  
  delay(200);
  while (CANbus.available())
  {
    CANbus.read(msg);
  }
  
  arm_MC(node_id);
  delay(500);
  
  send_statusword_request(node_id);
  delay(200);
  
  while (CANbus.available())
  {
    CANbus.read(msg);
    if (PRINT)
    {
      print_CAN_message(msg);
    }
    
    //previously searched for a 1A0 node id but this isn't always the node id sent when the controller gets armed
    if (msg.buf[4] == 55 && msg.buf[5] == 22)  //37 and 16 in hex
    {
      //were armed. lets go
      ret = 1;
    }
    
  }
  
  return ret;
}

int query_voltage_level(int node_id)
{
  CAN_message_t msg;
  int ret = 0;
  
  //send message to find voltage level
  msg.id = 0x600 + node_id;
  msg.ext = 0;
  msg.len = 4;
  msg.timeout = 0;
  //
  msg.buf[0] = 0x40;
  msg.buf[2] = 0x22;
  msg.buf[1] = 0x00;
  msg.buf[3] = 0x01;
  //
  msg.buf[5] = 0;
  msg.buf[4] = 0;
  msg.buf[6] = 0;
  msg.buf[7] = 0;

  //write message
  if (CANbus.write(msg) == 0)
  {
    ret = ERROR_CAN_WRITE;
  }
  else 
  {
  //it will stop responding but i want to test
    //print_CAN_message(msg);
    //Serial.println("waiting for response");
    //delayMicroseconds(WAIT_FOR_RESPONSE_TIME_FAST_US);
    while (CANbus.read(msg) == 0)
    {
      //wait for handshake
    }
    
    memcpy((char *)(&ret), &(msg.buf[4]),  1);
    memcpy((char *)(&ret) + 1, &(msg.buf[5]),  1);
    
    //print_CAN_message(msg);
    //Serial.println(ret);
  }

  return ret;
}

int shutdown_MC(int node_id)
{
  CAN_message_t msg;
  int ret = 0;

  //tell MC's to go to shutdown state
  msg.id = 0x600 + node_id;
  msg.ext = 0;
  msg.len = 6;
  msg.timeout = 0;
  //
  msg.buf[0] = 0x2B;  //68
  msg.buf[2] = 0x60;  //60
  msg.buf[1] = 0x40;
  msg.buf[3] = 0x00;
  //
  msg.buf[5] = 0x00;
  msg.buf[4] = 0b00000110;
  msg.buf[6] = 0;
  msg.buf[7] = 0;

  if (CANbus.write(msg) == 0)
  {
    ret = ERROR_CAN_WRITE;
    if (PRINT)
    {
      Serial.println("ERROR: CAN Write.");
      Serial.print("Node id: ");
      Serial.println(node_id);
    }
    
  }
  else {
    //print_CAN_message(msg);
    delayMicroseconds(WAIT_FOR_RESPONSE_TIME_SLOW_US);
    if (CANbus.read(msg) != 0)
    {
      if (PRINT)
      {
        Serial.println("Received Shutdown Confirmation");
      }
      
    }
    else
    {
      if (PRINT)
      {
        Serial.println("DID NOT Receive Shutdown Confirmation");
      }
      
    }
  }

  return ret; 
}




