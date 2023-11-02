#ifndef CHITECH_LBTS_TRANSIENT_SOLVER_H
#define CHITECH_LBTS_TRANSIENT_SOLVER_H

#include "Cc_DO_KEigenvalue/lbkes_k_eigenvalue_solver.h"
#include "math/chi_math_time_stepping.h"

typedef chi_mesh::sweep_management::SweepChunk SweepChunk;

namespace lbs
{

//################################################################### Class def
/**A transient neutral particle transport solver.
 *
\author Zachary Hardy.*/
class DiscOrdTransientSolver : public DiscOrdKEigenvalueSolver
{
public:
  chi_math::SteppingMethod method = chi_math::SteppingMethod::CRANK_NICOLSON;

  /// Options for initial condition normalization
  enum class NormalizationMethod
  {
    TOTAL_POWER = 0,    ///< Total reactor power
    POWER_DENSITY = 1,  ///< Power density
    NONE = 2            ///< No normalization
  };

  struct Options
  {
    int verbosity_level = 1;

    bool inhibit_advance = false;
    double t_final = 0.1;
    int max_time_steps = 10;
    std::string console_call_back_function;

    bool scale_fission_xs = false;
    NormalizationMethod normalization_method = NormalizationMethod::TOTAL_POWER;
  }transient_options_;

  /**Temporal domain and discretization information.*/
  double dt_ = 2.0e-3;
  double time_ = 0.0;

protected:
  /**Previous time step vectors.*/
  std::vector<double> phi_prev_local_;
  std::vector<double> precursor_prev_local_;
  std::vector<std::vector<double>> psi_prev_local_;

  /**Fission rate vector*/
  std::vector<double> fission_rate_local_;

public:
  explicit DiscOrdTransientSolver(const std::string& in_text_name);

  //01
  void Initialize() override;
  //02
  void Execute() override;
  void Step() override;
  void Advance() override;

  //Iterative operations
  std::shared_ptr<SweepChunk> SetTransientSweepChunk(LBSGroupset& groupset);

  double ComputeBeta();
  void   PostStepCallBackFunction() const;

  //precursors
  void StepPrecursors();

  virtual ~DiscOrdTransientSolver() override;
};

}//namespace lbs

#endif //CHITECH_LBTS_TRANSIENT_SOLVER_H
