#include "Configuration.hpp"
#include <boost/test/unit_test.hpp>

#include <rapidjson/document.h>
BOOST_AUTO_TEST_SUITE(ConfigurationTest)

BOOST_AUTO_TEST_CASE(HomeLocationSize)
{
  {
    std::string json(
      "{\"HomeLocation\": [{\"x\":5, \"y\":5},{\"x\":6, \"y\":6}]}");
    rapidjson::Document d;
    d.Parse(json.c_str());
    auto response = configuration::parseHomeCoordinates(d.GetObject());
    BOOST_CHECK(response.size() == 2);
    BOOST_CHECK(response[0].x == 5);
    BOOST_CHECK(response[0].y == 5);
    BOOST_CHECK(response[1].x == 6);
    BOOST_CHECK(response[1].y == 6);
  }

  {
    std::string json("{\"HomeLocation\": [[5,5],[6,6]]}");
    rapidjson::Document d;
    d.Parse(json.c_str());
    auto response = configuration::parseHomeCoordinates(d.GetObject());
    BOOST_CHECK(response.size() == 2);
    BOOST_CHECK(response[0].x == 5);
    BOOST_CHECK(response[0].y == 5);
    BOOST_CHECK(response[1].x == 6);
    BOOST_CHECK(response[1].y == 6);
  }
  {
    std::string json("{\"HomeLocation\": [{\"x\":5, \"y\":5},[6,6]]}");
    rapidjson::Document d;
    d.Parse(json.c_str());
    auto response = configuration::parseHomeCoordinates(d.GetObject());
    BOOST_CHECK(response.size() == 2);
    BOOST_CHECK(response[0].x == 5);
    BOOST_CHECK(response[0].y == 5);
    BOOST_CHECK(response[1].x == 6);
    BOOST_CHECK(response[1].y == 6);
  }

  {
    std::string json("{\"HomeLocation\": {\"x\":5, \"y\":5}}");
    rapidjson::Document d;
    d.Parse(json.c_str());
    auto response = configuration::parseHomeCoordinates(d.GetObject());
    BOOST_CHECK(response.size() == 1);
    BOOST_CHECK(response[0].x == 5);
    BOOST_CHECK(response[0].y == 5);
  }
  {
    std::string json("{\"HomeLocation\": [5,5]}");
    rapidjson::Document d;
    d.Parse(json.c_str());
    auto response = configuration::parseHomeCoordinates(d.GetObject());
    BOOST_CHECK(response.size() == 1);
    BOOST_CHECK(response[0].x == 5);
    BOOST_CHECK(response[0].y == 5);
  }
}

BOOST_AUTO_TEST_CASE(Drones)
{
  {
    std::string json("{\"Drones\": [5,5,5]}");
    rapidjson::Document d;
    d.Parse(json.c_str());
    auto response = configuration::parseDrones(d.GetObject());
    BOOST_CHECK(response.size() == 1);
    BOOST_CHECK(response[0].size() == 3);
    BOOST_CHECK(response[0][0] == 5);
    BOOST_CHECK(response[0][1] == 5);
    BOOST_CHECK(response[0][2] == 5);
  }
  {
    std::string json("{\"Drones\": [5]}");
    rapidjson::Document d;
    d.Parse(json.c_str());
    auto response = configuration::parseDrones(d.GetObject());
    BOOST_CHECK(response.size() == 1);
    BOOST_CHECK(response[0].size() == 1);
    BOOST_CHECK(response[0][0] == 5);
  }
  {
    std::string json("{\"Drones\": [[5,5,5], [5,5,5]]}");
    rapidjson::Document d;
    d.Parse(json.c_str());
    auto response = configuration::parseDrones(d.GetObject());
    BOOST_CHECK(response.size() == 2);
    BOOST_CHECK(response[0].size() == 3);
    BOOST_CHECK(response[1].size() == 3);
    BOOST_CHECK(response[0][0] == 5);
    BOOST_CHECK(response[0][1] == 5);
    BOOST_CHECK(response[0][2] == 5);
    BOOST_CHECK(response[1][0] == 5);
    BOOST_CHECK(response[1][1] == 5);
    BOOST_CHECK(response[1][2] == 5);
  }
}

BOOST_AUTO_TEST_CASE(TargetCount)
{
  {
    std::string json("{\"TargetCount\": [5, 6, 7]}");
    rapidjson::Document d;
    d.Parse(json.c_str());
    auto response = configuration::parseTargetCount(d.GetObject());
    BOOST_CHECK(response.size() == 3);
    BOOST_CHECK(response[0] == 5);
    BOOST_CHECK(response[1] == 6);
    BOOST_CHECK(response[2] == 7);
  }
  {
    std::string json("{\"TargetCount\": 14}");
    rapidjson::Document d;
    d.Parse(json.c_str());
    auto response = configuration::parseTargetCount(d.GetObject());
    BOOST_CHECK(response.size() == 1);
    BOOST_CHECK(response[0] == 14);
  }
}

BOOST_AUTO_TEST_CASE(Targets)
{
  {
    std::string json("{\"Targets\": [[5,5],[6,6],[7,7]]}");
    rapidjson::Document d;
    d.Parse(json.c_str());
    auto response = configuration::parseTargets(d.GetObject());
    BOOST_CHECK(response.size() == 1);
    BOOST_CHECK(response[0].size() == 3);
    BOOST_CHECK(response[0][0].x == 5);
    BOOST_CHECK(response[0][0].y == 5);
    BOOST_CHECK(response[0][1].x == 6);
    BOOST_CHECK(response[0][1].y == 6);
    BOOST_CHECK(response[0][2].x == 7);
    BOOST_CHECK(response[0][2].y == 7);
  }
  {
    std::string json(
      "{\"Targets\": [[5, 5],{\"x\":6,\"y\":6},{\"x\":7,\"y\":7}]}");
    rapidjson::Document d;
    d.Parse(json.c_str());
    auto response = configuration::parseTargets(d.GetObject());
    BOOST_CHECK(response.size() == 1);
    BOOST_CHECK(response[0].size() == 3);
    BOOST_CHECK(response[0][0].x == 5);
    BOOST_CHECK(response[0][0].y == 5);
    BOOST_CHECK(response[0][1].x == 6);
    BOOST_CHECK(response[0][1].y == 6);
    BOOST_CHECK(response[0][2].x == 7);
    BOOST_CHECK(response[0][2].y == 7);
  }
  {
    std::string json(
      "{\"Targets\": [[[5,5],[6,6],[7,7]], [[5,5],[6,6],[7,7]]]}");
    rapidjson::Document d;
    d.Parse(json.c_str());
    auto response = configuration::parseTargets(d.GetObject());
    BOOST_CHECK(response.size() == 2);
    BOOST_CHECK(response[0].size() == 3);
    BOOST_CHECK(response[1].size() == 3);
    BOOST_CHECK(response[0][0].x == 5);
    BOOST_CHECK(response[0][0].y == 5);
    BOOST_CHECK(response[0][1].x == 6);
    BOOST_CHECK(response[0][1].y == 6);
    BOOST_CHECK(response[0][2].x == 7);
    BOOST_CHECK(response[0][2].y == 7);
    BOOST_CHECK(response[1][0].x == 5);
    BOOST_CHECK(response[1][0].y == 5);
    BOOST_CHECK(response[1][1].x == 6);
    BOOST_CHECK(response[1][1].y == 6);
    BOOST_CHECK(response[1][2].x == 7);
    BOOST_CHECK(response[1][2].y == 7);
  }
  {
    std::string json(
      "{\"Targets\": [[{\"x\":5,\"y\":5},{\"x\":6,\"y\":6},[7,7]], "
      "[[5,5],[6,6],[7,7]]]}");
    rapidjson::Document d;
    d.Parse(json.c_str());
    auto response = configuration::parseTargets(d.GetObject());
    BOOST_CHECK(response.size() == 2);
    BOOST_CHECK(response[0].size() == 3);
    BOOST_CHECK(response[1].size() == 3);
    BOOST_CHECK(response[0][0].x == 5);
    BOOST_CHECK(response[0][0].y == 5);
    BOOST_CHECK(response[0][1].x == 6);
    BOOST_CHECK(response[0][1].y == 6);
    BOOST_CHECK(response[0][2].x == 7);
    BOOST_CHECK(response[0][2].y == 7);
    BOOST_CHECK(response[1][0].x == 5);
    BOOST_CHECK(response[1][0].y == 5);
    BOOST_CHECK(response[1][1].x == 6);
    BOOST_CHECK(response[1][1].y == 6);
    BOOST_CHECK(response[1][2].x == 7);
    BOOST_CHECK(response[1][2].y == 7);
  }
}

BOOST_AUTO_TEST_CASE(ValueFunctionTest)
{
  {
    std::string json("{\"ValueFunction\": \"n^2\"}");
    rapidjson::Document d;
    d.Parse(json.c_str());
    auto response = configuration::parseValueFunctions(d.GetObject());
    BOOST_CHECK(response.size() == 1);
    BOOST_CHECK(response[0].size() == 1);
    BOOST_CHECK(response[0][0].valueFunction == ValueFunction::nn);
    BOOST_CHECK(response[0][0].percentage == 1);
  }
  {
    std::string json("{\"ValueFunction\": [\"n^2\", \"n^3\", \"n\"]}");
    rapidjson::Document d;
    d.Parse(json.c_str());
    auto response = configuration::parseValueFunctions(d.GetObject());
    BOOST_CHECK(response.size() == 3);
    BOOST_CHECK(response[0].size() == 1);
    BOOST_CHECK(response[0][0].valueFunction == ValueFunction::nn);
    BOOST_CHECK(response[0][0].percentage == 1);
    BOOST_CHECK(response[1].size() == 1);
    BOOST_CHECK(response[1][0].valueFunction == ValueFunction::nnn);
    BOOST_CHECK(response[1][0].percentage == 1);
    BOOST_CHECK(response[2].size() == 1);
    BOOST_CHECK(response[2][0].valueFunction == ValueFunction::n);
    BOOST_CHECK(response[2][0].percentage == 1);
  }
  {
    std::string json(
      "{\"ValueFunction\": [[\"n^2\", 0.5], [\"n^3\", 0.3], [\"n\", 0.2]]}");
    rapidjson::Document d;
    d.Parse(json.c_str());
    auto response = configuration::parseValueFunctions(d.GetObject());
    BOOST_CHECK(response.size() == 1);
    BOOST_CHECK(response[0].size() == 3);
    BOOST_CHECK(response[0][0].valueFunction == ValueFunction::nn);
    BOOST_CHECK(response[0][0].percentage == .5);
    BOOST_CHECK(response[0][1].valueFunction == ValueFunction::nnn);
    BOOST_CHECK(response[0][1].percentage == .3);
    BOOST_CHECK(response[0][2].valueFunction == ValueFunction::n);
    BOOST_CHECK(response[0][2].percentage == .2);
  }
  {
    std::string json(
      "{\"ValueFunction\": [[[\"n^2\", 0.5], [\"n^3\", 0.3], [\"n\", "
      "0.2]],[[\"n^2\", 0.5], [\"n^3\", 0.3], [\"n\", 0.2]]]}");
    rapidjson::Document d;
    d.Parse(json.c_str());
    auto response = configuration::parseValueFunctions(d.GetObject());
    BOOST_CHECK(response.size() == 2);
    BOOST_CHECK(response[0].size() == 3);
    BOOST_CHECK(response[0][0].valueFunction == ValueFunction::nn);
    BOOST_CHECK(response[0][0].percentage == .5);
    BOOST_CHECK(response[0][1].valueFunction == ValueFunction::nnn);
    BOOST_CHECK(response[0][1].percentage == .3);
    BOOST_CHECK(response[0][2].valueFunction == ValueFunction::n);
    BOOST_CHECK(response[0][2].percentage == .2);

    BOOST_CHECK(response[1].size() == 3);
    BOOST_CHECK(response[1][0].valueFunction == ValueFunction::nn);
    BOOST_CHECK(response[1][0].percentage == .5);
    BOOST_CHECK(response[1][1].valueFunction == ValueFunction::nnn);
    BOOST_CHECK(response[1][1].percentage == .3);
    BOOST_CHECK(response[1][2].valueFunction == ValueFunction::n);
    BOOST_CHECK(response[1][2].percentage == .2);
  }
}

BOOST_AUTO_TEST_CASE(DiffPercentage)
{
  {
    std::string json("{\"DiffPercentage\": 0.01}");
    rapidjson::Document d;
    d.Parse(json.c_str());
    auto response = configuration::parseDiff(d.GetObject());
    BOOST_CHECK(response.size() == 1);
    BOOST_CHECK(response[0] == .01);
  }
  {
    std::string json("{\"DiffPercentage\": [0.01, 0.02, 0.03]}");
    rapidjson::Document d;
    d.Parse(json.c_str());
    auto response = configuration::parseDiff(d.GetObject());
    BOOST_CHECK(response.size() == 3);
    BOOST_CHECK(response[0] == .01);
    BOOST_CHECK(response[1] == .02);
    BOOST_CHECK(response[2] == .03);
  }
}

BOOST_AUTO_TEST_SUITE_END()