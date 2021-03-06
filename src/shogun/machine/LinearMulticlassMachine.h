/*
 * This software is distributed under BSD 3-clause license (see LICENSE file).
 *
 * Authors: Sergey Lisitsyn, Heiko Strathmann, Chiyuan Zhang, Fernando Iglesias,
 *          Evan Shelhamer, Viktor Gal, Soeren Sonnenburg, Yuyu Zhang,
 *          Evangelos Anagnostopoulos
 */

#ifndef _LINEARMULTICLASSMACHINE_H___
#define _LINEARMULTICLASSMACHINE_H___

#include <shogun/lib/config.h>

#include <shogun/lib/common.h>
#include <shogun/features/DotFeatures.h>
#include <shogun/machine/LinearMachine.h>
#include <shogun/machine/MulticlassMachine.h>

namespace shogun
{

class CDotFeatures;
class CLinearMachine;
class CMulticlassStrategy;

/** @brief generic linear multiclass machine */
class CLinearMulticlassMachine : public CMulticlassMachine
{
	public:
		/** default constructor  */
		CLinearMulticlassMachine() : CMulticlassMachine(), m_features(NULL)
		{
			SG_ADD((CSGObject**)&m_features, "m_features", "Feature object.");
		}

		/** standard constructor
		 * @param strategy multiclass strategy
		 * @param features features
		 * @param machine linear machine
		 * @param labs labels
		 */
		CLinearMulticlassMachine(CMulticlassStrategy *strategy, CFeatures* features, CMachine* machine, CLabels* labs) :
			CMulticlassMachine(strategy, machine,labs), m_features(NULL)
		{
			set_features(features->as<CDotFeatures>());
			SG_ADD(&m_features, "m_features", "Feature object.");
		}

		/** destructor */
		virtual ~CLinearMulticlassMachine()
		{
			SG_UNREF(m_features);
		}

		/** get name */
		virtual const char* get_name() const
		{
			return "LinearMulticlassMachine";
		}

		/** set features
		 *
		 * @param f features
		 */
		void set_features(CDotFeatures* f)
		{
			SG_REF(f);
			SG_UNREF(m_features);
			m_features = f;

			for (index_t i=0; i<m_machines->get_num_elements(); i++)
			{
				auto machine = m_machines->get_element(i)->as<CLinearMachine>();
				machine->set_features(f);
				SG_UNREF(machine);
			}
		}

		/** get features
		 *
		 * @return features
		 */
		CDotFeatures* get_features() const
		{
			SG_REF(m_features);
			return m_features;
		}

	protected:

		/** init machine for train with setting features */
		virtual bool init_machine_for_train(CFeatures* data)
		{
			if (!m_machine)
				SG_ERROR("No machine given in Multiclass constructor\n")

			if (data)
				set_features((CDotFeatures*)data);

			m_machine->as<CLinearMachine>()->set_features(m_features);

			return true;
		}

		/** init machines for applying with setting features */
		virtual bool init_machines_for_apply(CFeatures* data)
		{
			if (data)
				set_features((CDotFeatures*)data);

			for (int32_t i=0; i<m_machines->get_num_elements(); i++)
			{
				auto machine = m_machines->get_element(i)->as<CLinearMachine>();
				ASSERT(m_features)
				ASSERT(machine)
				machine->set_features(m_features);
				SG_UNREF(machine);
			}

			return true;
		}

		/** check features availability */
		virtual bool is_ready()
		{
			if (m_features)
				return true;

			return false;
		}

		/** construct linear machine from given linear machine */
		virtual CMachine* get_machine_from_trained(CMachine* machine) const
		{
			return new CLinearMachine(machine->as<CLinearMachine>());
		}

		/** get number of rhs feature vectors */
		virtual int32_t get_num_rhs_vectors() const
		{
			return m_features->get_num_vectors();
		}

		/** set subset to the features of the machine, deletes old one
		 *
		 * @param subset subset instance to set
		 */
		virtual void add_machine_subset(SGVector<index_t> subset)
		{
			/* changing the subset structure to use subset stacks. This might
			 * have to be revised. Heiko Strathmann */
			m_features->add_subset(subset);
		}

		/** deletes any subset set to the features of the machine */
		virtual void remove_machine_subset()
		{
			/* changing the subset structure to use subset stacks. This might
			 * have to be revised. Heiko Strathmann */
			m_features->remove_subset();
		}

		/** Stores feature data of underlying model. Does nothing because
		 * Linear machines store the normal vector of the separating hyperplane
		 * and therefore the model anyway
		 */
		virtual void store_model_features() {}

	protected:

		/** features */
		CDotFeatures* m_features;
};
}
#endif
