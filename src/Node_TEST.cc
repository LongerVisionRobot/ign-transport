/*
 * Copyright (C) 2014 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <robot_msgs/stringmsg.pb.h>
#include <string>
#include <thread>
#include "gtest/gtest.h"
#include "ignition/transport/Node.hh"

using namespace ignition;

bool cbExecuted;
bool cb2Executed;
std::string topic = "foo";
std::string data = "bar";

static void s_sleep(int msecs)
{
  struct timespec t;
  t.tv_sec = msecs / 1000;
  t.tv_nsec = (msecs % 1000) * 1000000;
  nanosleep(&t, nullptr);
}

//////////////////////////////////////////////////
/// \brief Function is called everytime a topic update is received.
void cb(const std::string &_topic, const robot_msgs::StringMsg &_msg)
{
  assert(_topic != "");

  EXPECT_EQ(_msg.data(), data);
  cbExecuted = true;
}

//////////////////////////////////////////////////
/// \brief Function is called everytime a topic update is received.
void cb2(const std::string &_topic, const robot_msgs::StringMsg &_msg)
{
  assert(_topic != "");

  EXPECT_EQ(_msg.data(), data);
  cb2Executed = true;
}

//////////////////////////////////////////////////
void CreateSubscriber()
{
  transport::Node node;
  EXPECT_EQ(node.Subscribe(topic, cb), 0);
  while (!cbExecuted)
    s_sleep(100);
}

//////////////////////////////////////////////////
TEST(DiscZmqTest, PubWithoutAdvertise)
{
  robot_msgs::StringMsg msg;
  msg.set_data(data);

  transport::Node node;

  // Publish some data on topic without advertising it first.
  EXPECT_NE(node.Publish(topic, msg), 0);
}

//////////////////////////////////////////////////
TEST(DiscZmqTest, PubSubSameThread)
{
  cbExecuted = false;
  robot_msgs::StringMsg msg;
  msg.set_data(data);

  transport::Node node;
  node.Advertise(topic);

  EXPECT_EQ(node.Subscribe(topic, cb), 0);
  s_sleep(100);

  // Publish a first message.
  EXPECT_EQ(node.Publish(topic, msg), 0);
  s_sleep(100);

  // Check that the msg was received.
  EXPECT_TRUE(cbExecuted);
  cbExecuted = false;

  // Publish a second message on topic.
  EXPECT_EQ(node.Publish(topic, msg), 0);
  s_sleep(100);

  // Check that the data was received.
  EXPECT_TRUE(cbExecuted);
  cbExecuted = false;

  node.Unadvertise(topic);

  // Publish a third message.
  EXPECT_NE(node.Publish(topic, msg), 0);
  s_sleep(100);
  EXPECT_FALSE(cbExecuted);
  cbExecuted = false;
}

//////////////////////////////////////////////////
TEST(DiscZmqTest, PubSubDifferentThreads)
{
  cbExecuted = false;
  robot_msgs::StringMsg msg;
  msg.set_data(data);

  transport::Node node;
  node.Advertise(topic);
  s_sleep(100);

  // Subscribe to topic in a different thread and wait until the callback is
  // received.
  std::thread subscribeThread(CreateSubscriber);
  s_sleep(100);

  // Publish a msg on topic.
  EXPECT_EQ(node.Publish(topic, msg), 0);
  s_sleep(100);

  // Wait until the subscribe thread finishes.
  subscribeThread.join();

  // Check that the message was received.
  EXPECT_TRUE(cbExecuted);
  cbExecuted = false;
}

//////////////////////////////////////////////////
TEST(DiscZmqTest, TwoSubscribersSameThread)
{
  cbExecuted = false;
  cb2Executed = false;
  robot_msgs::StringMsg msg;
  msg.set_data(data);

  transport::Node node1;
  transport::Node node2;

  node1.Advertise(topic);
  s_sleep(100);

  // Subscribe to topic in node1.
  EXPECT_EQ(node1.Subscribe(topic, cb), 0);
  s_sleep(100);

  // Subscribe to topic in node2.
  EXPECT_EQ(node2.Subscribe(topic, cb2), 0);
  s_sleep(100);

  EXPECT_EQ(node1.Publish(topic, msg), 0);
  s_sleep(100);

  // Check that the msg was received by node1.
  EXPECT_TRUE(cbExecuted);
  cbExecuted = false;

  // Check that the msg was received by node2.
  EXPECT_TRUE(cb2Executed);
  cb2Executed = false;

  node1.Unadvertise(topic);

  // Publish a third message
  EXPECT_NE(node1.Publish(topic, msg), 0);
  s_sleep(100);
  EXPECT_FALSE(cbExecuted);
  EXPECT_FALSE(cb2Executed);
  cbExecuted = false;
  cb2Executed = false;
}

//////////////////////////////////////////////////
int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
