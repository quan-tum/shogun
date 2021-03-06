/*
 * This software is distributed under BSD 3-clause license (see LICENSE file).
 *
 * Authors: Shell Hu, Fernando Iglesias, Viktor Gal, Yuyu Zhang, 
 *          Sergey Lisitsyn, Bjoern Esser
 */

#ifndef _PRIMAL_MOSEK_SOSVM__H__
#define _PRIMAL_MOSEK_SOSVM__H__

#include <shogun/lib/config.h>

#ifdef USE_MOSEK

#include <shogun/machine/LinearStructuredOutputMachine.h>
#include <shogun/mathematics/Mosek.h>

namespace shogun
{

/**
 * @brief Class PrimalMosekSOSVM that implements the optimization
 * algorithm for structured output (SO) problems presented in [1] for SVM
 * learning. The optimization problem is solved using a cutting plane algorithm.
 *
 * [1] Tsochantaridis, I., Hofmann, T., Joachims, T., Altun, Y.
 *     Support Vector Machine Learning for Interdependent and Structured Ouput
 *     Spaces.
 *     http://www.cs.cornell.edu/People/tj/publications/tsochantaridis_etal_04a.pdf
 */
class CPrimalMosekSOSVM : public CLinearStructuredOutputMachine
{
	public:
		/** default constructor */
		CPrimalMosekSOSVM();

		/** standard constructor
		 *
		 * @param model structured model with application specific functions
		 * @param labs structured labels
		 */
		CPrimalMosekSOSVM(CStructuredModel* model, CStructuredLabels* labs);

		/** destructor */
		~CPrimalMosekSOSVM();

		/** get slacks
		 *
		 * @return slacks
		 */
		inline SGVector< float64_t > get_slacks() const { return m_slacks; }

		/** compute the primal objective value
		 *
		 * @return primal objective value.
		 */
		virtual float64_t compute_primal_objective() const;

		/** @return name of SGSerializable */
		virtual const char* get_name() const { return "PrimalMosekSOSVM"; }

		/** get classifier type
		 *
		 * @return classifier type CT_PRIMALMOSEKSOSVM
		 */
		virtual EMachineType get_classifier_type();

		/** set regularization constant C
		 *
		 * @param C regularization constant
		 */
		void set_regularization(float64_t C);

		/** set epsilon
		 *
		 * @param epsilon if slack_i > max_slack_i + epsilon, add to cutting plane set
		 */
		void set_epsilon(float64_t epsilon);

		/** set lower bounds
		 * @param lb lower bounds
		 */
		void set_lower_bounds(SGVector< float64_t > lb);

		/** set upper bounds
		 * @param ub upper bounds
		 */
		void set_upper_bounds(SGVector< float64_t > ub);

	protected:
		/** train primal SO-SVM
		 *
		 * @param data training data
		 * @return whether the training was successful
		 */
		virtual bool train_machine(CFeatures* data = NULL);

	private:
		void init();

		/** computes the result of TODO equation
		 *
		 * @param result CResultSet structure with any argmax output
		 * @return result of the operation
		 */
		float64_t compute_loss_arg(CResultSet* result) const;

		/** insert element in the list of argmax results
		 *
		 * @param result_list list of CResultSet
		 * @param result element to insert in the list
		 */
		bool insert_result(CList* result_list, CResultSet* result) const;

		/** introduces a new constraint of type Ax <= b in the
		 * optimization problem. Remember that each row i in A takes the
		 * form
		 *
		 * \f[
		 * \Psi(\vec{x}_i,{\hat{y}_i}) - \Psi(\vec{x}_i,y_i) ~|~-\delta_{ji}
		 *\f]
		 *
		 * and the corresponding element in b is
		 *
		 * \f[
		 * -\Delta(y, \hat{y}_i)
		 *\f]
		 *
		 * @param mosek MOSEK optimization problem instance
		 * @param result structure with numerical information of the
		 * constraint
		 * @param con_idx row index in A for this new constraint
		 * @param train_idx training example associated to this
		 * constraint
		 *
		 * @return whether the new constraint has been succesfully added
		 */
		bool add_constraint(CMosek* mosek, CResultSet* result, index_t con_idx, index_t train_idx) const;

	private:
		/** slack variables associated to each training example */
		SGVector< float64_t > m_slacks;

		/** primal objective value */
		float64_t po_value;

		/** regularization constant */
		float64_t m_regularization;

		/** epsilon */
		float64_t m_epsilon;

		/** lower bounds */
		SGVector< float64_t > m_lb;

		/** upper bounds */
		SGVector< float64_t > m_ub;

}; /* class CPrimalMosekSOSVM */

} /* namespace shogun */

#endif /* USE_MOSEK */
#endif /* _PRIMAL_MOSEK_SOSVM__H__ */
