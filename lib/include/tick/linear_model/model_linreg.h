//
// Created by Stéphane GAIFFAS on 12/12/2015.
//

#ifndef LIB_INCLUDE_TICK_LINEAR_MODEL_MODEL_LINREG_H_
#define LIB_INCLUDE_TICK_LINEAR_MODEL_MODEL_LINREG_H_

// License: BSD 3 clause

#include "tick/base_model/model_generalized_linear.h"
#include "tick/base_model/model_lipschitz.h"

#include <cereal/types/base_class.hpp>

template <class T>
class DLL_PUBLIC TModelLinReg : public virtual TModelGeneralizedLinear<T>,
                                public TModelLipschitz<T> {
 private:
  std::string clazz = "TModelGeneralizedLinearWithIntercepts<" +
                      std::string(typeid(T).name()) + ">";

 protected:
  using TModelLipschitz<T>::ready_lip_consts;
  using TModelLipschitz<T>::lip_consts;
  using TModelGeneralizedLinear<T>::compute_features_norm_sq;
  using TModelGeneralizedLinear<T>::n_samples;
  using TModelGeneralizedLinear<T>::features_norm_sq;
  using TModelGeneralizedLinear<T>::fit_intercept;

 public:
  using TModelGeneralizedLinear<T>::get_label;
  using TModelGeneralizedLinear<T>::use_intercept;
  using TModelGeneralizedLinear<T>::get_inner_prod;

 public:
  TModelLinReg(const std::shared_ptr<BaseArray2d<T> > features,
               const std::shared_ptr<SArray<T> > labels,
               const bool fit_intercept, const int n_threads = 1);

  virtual ~TModelLinReg() {}

  virtual const char *get_class_name() const { return clazz.c_str(); }

  T sdca_dual_min_i(const ulong i, const T dual_i,
                    const Array<T> &primal_vector,
                    const T previous_delta_dual_i, T l_l2sq) override;

  T loss_i(const ulong i, const Array<T> &coeffs) override;

  T grad_i_factor(const ulong i, const Array<T> &coeffs) override;

  void compute_lip_consts() override;

  template <class Archive>
  void serialize(Archive &ar) {
    ar(cereal::make_nvp(
        "ModelGeneralizedLinear",
        typename cereal::virtual_base_class<TModelGeneralizedLinear<T> >(
            this)));
    ar(cereal::make_nvp(
        "ModelLipschitz",
        typename cereal::base_class<TModelLipschitz<T> >(this)));
  }
};

using ModelLinReg = TModelLinReg<double>;

using ModelLinRegDouble = TModelLinReg<double>;
CEREAL_SPECIALIZE_FOR_ALL_ARCHIVES(ModelLinRegDouble,
                                   cereal::specialization::member_serialize)

using ModelLinRegFloat = TModelLinReg<float>;
CEREAL_SPECIALIZE_FOR_ALL_ARCHIVES(ModelLinRegFloat,
                                   cereal::specialization::member_serialize)

#endif  // LIB_INCLUDE_TICK_LINEAR_MODEL_MODEL_LINREG_H_
