/*	-------------------------------------------------------------------------------------------------------
	� 1991-2012 Take-Two Interactive Software and its subsidiaries.  Developed by Firaxis Games.  
	Sid Meier's Civilization V, Civ, Civilization, 2K Games, Firaxis Games, Take-Two Interactive Software 
	and their respective logos are all trademarks of Take-Two interactive Software, Inc.  
	All other marks and trademarks are the property of their respective owners.  
	All rights reserved. 
	------------------------------------------------------------------------------------------------------- */
#pragma once

#ifndef CIV5_POLICYAI_H
#define CIV5_POLICYAI_H

// Allocate array for 50% more policies than we have now (22)
//   May want to tune this number closer to shipping, though safe enough
//   given that each entry is only 8 bytes
#define SAFE_ESTIMATE_NUM_POLICIES 64

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  CLASS:      CvPolicyAI
//!  \brief		Handles policy AI decisions for one player
//
//!  Key Attributes:
//!  - Object created by CvPlayerPolicies (so one per player)
//!  - Design described in Doc\Tech\GameCoreDLL\PolicyAI.doc
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CvPolicyAI
{
public:
	CvPolicyAI(CvPlayerPolicies* currentPolicies);
	~CvPolicyAI(void);

	// Initialization
	void Reset();

	// Serialization routines
	void Read(FDataStream& kStream);
	void Write(FDataStream& kStream);

	// Establish weights for one flavor
	void AddFlavorWeights(FlavorTypes eFlavor, int iWeight, int iPropagationPercent);

	// Choose a player's next policy purchase
	int ChooseNextPolicy(CvPlayer* pPlayer);

	// Ideology
	void DoChooseIdeology(CvPlayer* pPlayer);
	void DoConsiderIdeologySwitch(CvPlayer* pPlayer);
	int GetBranchBuildingHappiness(CvPlayer* pPlayer, PolicyBranchTypes eBranch);
	int GetNumHappinessPolicies(CvPlayer* pPlayer, PolicyBranchTypes eBranch);

private:
	// Internal methods
	void WeightPrereqs(int* paiTempWeightst, int iPropagationPercent);
	void PropagateWeights(int iPolicy, int iWeight, int iPropagationPercent, int iPropagationLevel);
#ifdef AUI_POLICY_USE_DOUBLES
#ifdef AUI_POLICY_WEIGH_BRANCH_INCLUDES_ERA_DIFFERENCE
	double WeighBranch(CvPlayer* pPlayer, PolicyBranchTypes eBranch);
#else
#ifdef AUI_POLICY_WEIGH_BRANCH_INCLUDES_WONDER
	double WeighBranch(CvPlayer* pPlayer, PolicyBranchTypes eBranch);
#else
	double WeighBranch(PolicyBranchTypes eBranch);
#endif // AUI_POLICY_WEIGH_BRANCH_INCLUDES_WONDER
#endif // AUI_POLICY_WEIGH_BRANCH_INCLUDES_ERA_DIFFERENCE
#else
#ifdef AUI_POLICY_WEIGH_BRANCH_INCLUDES_ERA_DIFFERENCE
	int WeighBranch(CvPlayer* pPlayer, PolicyBranchTypes eBranch);
#else
#ifdef AUI_POLICY_WEIGH_BRANCH_INCLUDES_WONDER
	int WeighBranch(CvPlayer* pPlayer, PolicyBranchTypes eBranch);
#else
	int WeighBranch(PolicyBranchTypes eBranch);
#endif // AUI_POLICY_WEIGH_BRANCH_INCLUDES_WONDER
#endif // AUI_POLICY_WEIGH_BRANCH_INCLUDES_ERA_DIFFERENCE
#endif // AUI_POLICY_USE_DOUBLES
	bool IsBranchEffectiveInGame(PolicyBranchTypes eBranch);
#ifdef AUI_POLICY_MULTIPLY_FLAVOR_WEIGHT_FOR_UNIQUE_GREAT_PERSON
	double BoostFlavorDueToUniqueGP(CvPolicyEntry* pEntry);
	void UpdateUniqueGPVector(bool bAlwaysUpdate = true);
#endif // AUI_POLICY_MULTIPLY_FLAVOR_WEIGHT_FOR_UNIQUE_GREAT_PERSON

	// Logging functions
	void LogPossiblePolicies();
	void LogPolicyChoice(PolicyTypes ePolicy);
	void LogBranchChoice(PolicyBranchTypes eBranch);
	void LogIdeologyChoice(CvString &decisionState, int iWeightFreedom, int iWeightAutocracy, int iWeightOrder);
	CvString GetLogFileName(CvString& playerName) const;

	// Private data
	CvPlayerPolicies* m_pCurrentPolicies;
	CvWeightedVector<int, SAFE_ESTIMATE_NUM_POLICIES, true> m_PolicyAIWeights;
#ifdef AUI_POLICY_MULTIPLY_FLAVOR_WEIGHT_FOR_UNIQUE_GREAT_PERSON
	// vector index = Great Person type; true/false value = is it unique?
	std::vector<bool> m_bUniqueGreatPersons;
#endif // AUI_POLICY_MULTIPLY_FLAVOR_WEIGHT_FOR_UNIQUE_GREAT_PERSON

	// First NUM_POLICY_BRANCH_TYPE entries are branches; policies start after that
	CvWeightedVector<int, SAFE_ESTIMATE_NUM_POLICIES, true> m_AdoptablePolicies;

	// Locally cached GlobalAIDefines
	int m_iPolicyWeightPropagationLevels;
	int m_iPolicyWeightPercentDropNewBranch;
};

#endif //CIV5_POLICYAI_H