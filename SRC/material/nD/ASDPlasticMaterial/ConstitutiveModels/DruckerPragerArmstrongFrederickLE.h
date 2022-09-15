/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 1999, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** Commercial use of this program without express permission of the   **
** University of California, Berkeley, is strictly prohibited.  See   **
** file 'COPYRIGHT'  in main directory for information on usage and   **
** redistribution,  and for a DISCLAIMER OF ALL WARRANTIES.           **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** ****************************************************************** */
                                                                        
// Original implementation: José Abell (UANDES), Massimo Petracca (ASDEA)
//
// ASDPlasticMaterial
//
// Fully general templated material class for plasticity modeling


#include "../ClassicElastoplasticityGlobals.h"
#include "../ClassicElastoplasticMaterial.h"
#include "../MaterialInternalVariables.h"

//Yield Functions
#include "../YieldFunctions/DruckerPrager_YF.h"

//Plastic flow directions
#include "../PlasticFlowDirections/DruckerPragerDeviatoric_PF.h"

//Elasticity
#include "../ElasticityModels/LinearIsotropic3D_EL.h"

//Evolving variables
#include "../EvolvingVariables/ArmstrongFrederickTensor_EV.h"
#include "../EvolvingVariables/LinearHardeningScalar_EV.h"



#include <classTags.h>



// New materials are created by subclassing instances of the ClassicElastoplasticMaterial<.,.,.,.,>
// template class, with the appropriate components as template parameters.
// Heavy use of templating is made, therefore typedeffing is a friend in helping clear up the mess.

//Drucker Prager Model with linear hardening (DPAFLE)
class DruckerPragerArmstrongFrederickLE;  //This model we will define

//Activate pre_integration_callback to handle tension case
template< >
struct supports_pre_integration_callback<DruckerPragerArmstrongFrederickLE>
{
    static const bool value = true;
};

//Typedefs for internal variables list, yield function, and plastic flow function
typedef MaterialInternalVariables < ArmstrongFrederickTensor_EV, LinearHardeningScalar_EV> DPAFLEVarsType;
typedef DruckerPrager_YF < ArmstrongFrederickTensor_EV, LinearHardeningScalar_EV> DPAFLE_YFType;
typedef DruckerPragerDeviatoric_PF < ArmstrongFrederickTensor_EV, LinearHardeningScalar_EV> DPAFLE_PFType;

//Create a helpful typedef for the base class from which we will inherit to create the n
typedef ClassicElastoplasticMaterial <LinearIsotropic3D_EL,
        DPAFLE_YFType,
        DPAFLE_PFType,
        DPAFLEVarsType,
        ND_TAG_CEM_DruckerPragerArmstrongFrederickLE,
        DruckerPragerArmstrongFrederickLE
        > DPAFLEBase;

//Define the new class. We must provide two constructor and the evolving variables as data-members.
class DruckerPragerArmstrongFrederickLE : public DPAFLEBase
{
public:

    //First constructor, creates a material at its "ground state" from its parameters.
    DruckerPragerArmstrongFrederickLE(int tag_in, double k0_in, double ha_alpha, double cr_alpha, double H_k, double E, double nu, double rho_, double p0) ;

    // Second constructor is not called by the user, instead it is called when creating a copy of the
    // material. This must provide an initialization for the state variables and link the components
    // to these variables appropriately.
    DruckerPragerArmstrongFrederickLE(int tag_in, double rho, double p0, DPAFLE_YFType & yf,
                                      LinearIsotropic3D_EL & el,
                                      DPAFLE_PFType & pf,
                                      DPAFLEVarsType & vars);

    // Empty constructor for parallel
    DruckerPragerArmstrongFrederickLE() ;

    int pre_integration_callback(const DTensor2&, const DTensor2&, const DTensor2&, const DTensor4&, double, double, bool&);

    void Print(ostream& s, int flag);


    //The state variables.
private:
    ArmstrongFrederickTensor_EV alpha; // Backstress
    LinearHardeningScalar_EV k;     // Critical stress ratio (k = M under this formulation)
};

