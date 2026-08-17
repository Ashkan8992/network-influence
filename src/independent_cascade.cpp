#include "influence/independent_cascade.hpp"

#include <stdexcept>

namespace influence {

IndependentCascade::IndependentCascade(const Graph& graph, Configuration configuration): graph_(graph), configuration_(configuration) {

    if (configuration_.activation_probability < 0.0 ||
        configuration_.activation_probability > 1.0) {
        throw std::invalid_argument(
            "activation probability must be between 0 and 1"
        );
    }

    if (configuration_.simulations == 0) {
        throw std::invalid_argument(
            "number of simulations must be greater than zero"
        );
    }
}

}  // namespace influence
