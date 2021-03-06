#include "gtest/gtest.h"
#include "constructs/include/discrete_oracle.h"

namespace {

using mnyrve::constructs::DiscreteOracle;
using Eigen::VectorXd;
using Eigen::MatrixXd;
using Eigen::ArrayXd;

using std::map;
using std::string;

class DiscreteOracleTest : public ::testing::Test {

  protected:
    DiscreteOracleTest() {
    
    }

    void SetUp() override {

    }

    DiscreteOracle do1_{DiscreteOracle(4)};

    DiscreteOracle do2_{DiscreteOracle(MatrixXd::Ones(4, 1))};

    map<string, double> m = 
    {{"a", 2.0}, {"b", -1.5}, {"c", 0}, {"d", 100}};
    DiscreteOracle do3_{DiscreteOracle(m)};
};

TEST_F(DiscreteOracleTest, InitializationCheck) {

  EXPECT_EQ(do1_.GetNumRewards(), 4);
  EXPECT_EQ(do1_.GetRewards().rows(), 4);
  ASSERT_TRUE(do1_.GetRewards().isZero());
  ASSERT_TRUE(do1_.GetRewardMap().empty());

  EXPECT_EQ(do2_.GetNumRewards(), 4);
  EXPECT_EQ(do2_.GetRewards().rows(), 4);
  ASSERT_TRUE(do2_.GetRewards().isOnes());
  ASSERT_TRUE(do2_.GetRewardMap().empty());

  EXPECT_EQ(do3_.GetNumRewards(), 4);
  EXPECT_EQ(do3_.GetRewards().rows(), 4);
  ASSERT_TRUE(do3_.GetRewardMap().size() == 4);

}

TEST_F(DiscreteOracleTest, SetterCheck) {

  VectorXd reward1, reward2;
  reward1.resize(4, 1);
  reward2.resize(4, 1);
  reward1 << 1, 2, 3, 4;
  reward2 << 2, -1.5, 0, 100;

  do1_.SetRewards(reward1);
  ASSERT_TRUE(do1_.GetRewards().isApprox(reward1));
  do2_.SetRewards(reward2);
  ASSERT_TRUE(do2_.GetRewards().isApprox(reward2));
  ASSERT_TRUE(do3_.GetRewards().isApprox(reward2));

  do1_.SetRandomRewards();
  do2_.SetRandomRewards();
  do3_.SetRandomRewards();
  ASSERT_TRUE((do1_.GetRewards().array()>=ArrayXd::Zero(4)).all());
  ASSERT_TRUE((do1_.GetRewards().array()<=ArrayXd::Ones(4)).all());
  ASSERT_TRUE((do2_.GetRewards().array()>=ArrayXd::Zero(4)).all());
  ASSERT_TRUE((do2_.GetRewards().array()<=ArrayXd::Ones(4)).all());
  ASSERT_TRUE((do3_.GetRewards().array()>=ArrayXd::Zero(4)).all());
  ASSERT_TRUE((do3_.GetRewards().array()<=ArrayXd::Ones(4)).all());


  ASSERT_DEATH(do1_.SetRewards(MatrixXd::Random(4, 2)), "");
  ASSERT_DEATH(do2_.SetRewards(VectorXd::Random(5, 1)), "");
  ASSERT_DEATH(do3_.SetRewards(MatrixXd::Random(4, 4)), "");

} 

TEST_F(DiscreteOracleTest, OperatorCheck) {

  ASSERT_DOUBLE_EQ(do1_(0), 0);
  ASSERT_DOUBLE_EQ(do2_(0), 1);
  ASSERT_DOUBLE_EQ(do3_("a"), 2);
  ASSERT_DOUBLE_EQ(do3_("b"), -1.5);
  ASSERT_DOUBLE_EQ(do3_("c"), 0);
  ASSERT_DOUBLE_EQ(do3_("d"), 100);
  do1_(1) = 7;
  ASSERT_DOUBLE_EQ(do1_(1), 7);
  ASSERT_DOUBLE_EQ(do1_.GetRewards()(1, 0), 7);

}


} // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}



