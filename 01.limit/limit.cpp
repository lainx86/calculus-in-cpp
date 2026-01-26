#include <print>
#include <cmath>
#include <vector>

constexpr double EPSILON = 1e-12;
constexpr double OFFSET = 1.0;
constexpr size_t MAX_ITERATIONS = 25;
constexpr double CONVERGENCE_TOL = 1e-5;

// Class to store numerical limit evaluation results
class LimitResult {
private:
    bool exists_;
    double value_;
    double left_limit_;
    double right_limit_;
    double difference_;
    std::vector<double> left_sequence_;
    std::vector<double> right_sequence_;
    double (*func_)(double);
    double point_;

public:
    LimitResult(double (*f)(double), double a, double epsilon = EPSILON)
        : func_(f), point_(a)
    {
        // Construct sequence approaching a from the left (x → a⁻)
        double current = a - OFFSET;
        while (std::abs(a - current) > epsilon && left_sequence_.size() < MAX_ITERATIONS) {
            left_sequence_.push_back(current);
            current += (a - current) * 0.5;
        }
        
        // Construct sequence approaching a from the right (x → a⁺)
        current = a + OFFSET;
        while (std::abs(current - a) > epsilon && right_sequence_.size() < MAX_ITERATIONS) {
            right_sequence_.push_back(current);
            current -= (current - a) * 0.5;
        }
        
        // Evaluate one-sided limits numerically
        left_limit_  = !left_sequence_.empty()  ? f(left_sequence_.back())  : NAN;
        right_limit_ = !right_sequence_.empty() ? f(right_sequence_.back()) : NAN;
        
        // Limit existence verification
        if (!std::isfinite(left_limit_) || !std::isfinite(right_limit_)) {
            exists_ = false;
            value_ = NAN;
            difference_ = NAN;
            return;
        }
        
        difference_ = std::abs(left_limit_ - right_limit_);
        exists_ = (difference_ < CONVERGENCE_TOL);
        value_ = exists_ ? (left_limit_ + right_limit_) / 2.0 : NAN;
        return;
    }
    
    // Display numerical limit evaluation procedure
    void display_calculation() const {
        std::print("Evaluating  lim(x → {}) f(x)\n", point_);
        
        // Left-hand limit table
        std::print("\nLEFT-HAND APPROACH  (x → {}⁻)\n", point_);
        std::print("{:<5} | {:<18} | {:<18}\n", "Step", "x", "f(x)");
        std::print("{:-<48}\n", "");
        for (size_t i = 0; i < left_sequence_.size(); ++i) {
            double x = left_sequence_[i];
            double fx = func_(x);
            if (std::isfinite(fx)) {
                std::print("{:<5} | {:>16.12f} | {:>16.12f}\n", i + 1, x, fx);
            } else {
                std::print("{:<5} | {:>16.12f} | {:>18}\n", i + 1, x, "undefined");
            }
        }
        
        // Right-hand limit table
        std::print("\nRIGHT-HAND APPROACH (x → {}⁺)\n", point_);
        std::print("{:<5} | {:<18} | {:<18}\n", "Step", "x", "f(x)");
        std::print("{:-<48}\n", "");
        for (size_t i = 0; i < right_sequence_.size(); ++i) {
            double x = right_sequence_[i];
            double fx = func_(x);
            if (std::isfinite(fx)) {
                std::print("{:<5} | {:>16.12f} | {:>16.12f}\n", i + 1, x, fx);
            } else {
                std::print("{:<5} | {:>16.12f} | {:>18}\n", i + 1, x, "undefined");
            }
        }
        
        // Summary of results
        std::print("\n{:=<48}\n", "");
        if (std::isfinite(left_limit_) && std::isfinite(right_limit_)) {
            std::print("Left-hand limit  (lim x→{}⁻ f(x)) : {:.12f}\n", point_, left_limit_);
            std::print("Right-hand limit (lim x→{}⁺ f(x)) : {:.12f}\n", point_, right_limit_);
            std::print("Absolute difference               : {:.2e}\n", difference_);
        }
        
        if (exists_) {
            std::print("\nLIMIT EXISTS\n");
            std::print("lim(x → {}) f(x) = {:.5f}\n", point_, value_);
        } else {
            std::print("\nLIMIT DOES NOT EXIST\n");
            if (!std::isfinite(left_limit_) || !std::isfinite(right_limit_)) {
                std::print("Reason: function is undefined in the neighborhood of a\n");
            } else {
                std::print("Reason: one-sided limits are not equal\n");
            }
        }
    }
    
    // Getter methods
    double value() const { return value_; }
    bool exists() const { return exists_; }
    double left_limit() const { return left_limit_; }
    double right_limit() const { return right_limit_; }
    double difference() const { return difference_; }
    
    // Conversion operator for direct numerical access
    operator double() const { return value_; }
};

LimitResult limit(double(*f)(double), double a, double e = EPSILON) {
  return LimitResult(f, a, e);
}

// Test functions
double f(double x) {
  return (8 - (3 * x) + (12 * std::pow(x, 2)));
}

double f2(double t) {
  return (6 + (4 * t)) / (std::pow(t, 2) + 1);
}

double f3(double x) {
  return(std::pow(x, 2) - 25) / (std::pow(x, 2) + (2 * x) - 15);
}

int main() {
  double x = -5;
  double t = -3;
  auto lim1 = limit(f3, x);
  std::print("is the value exist? ");
  if (lim1.exists()) {
    std::print("Yeah, the limit is exist\n");
    std::print("The limit when x -> {} f(x) = {:.5f}\n", x, lim1.value());
    std::print("The value of the limit when approches form the left is: {}\n", lim1.left_limit());
    std::print("The value of the limit when approches from the right is : {}\n", lim1.right_limit());
    std::print("The difference between left and right limit is: {}\n", lim1.difference());

    //this is when you want to see calculation/approaching process
    lim1.display_calculation();
  } else {
    std::print("Nope\n");
  }
  return 0;
}