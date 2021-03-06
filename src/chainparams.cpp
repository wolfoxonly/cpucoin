// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"
#include "arith_uint256.h"

#include <assert.h>
#include <stdio.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Shanghai stock index closed at 3180.16, Apr. 12th, 2018.";
    const CScript genesisOutputScript = CScript() << ParseHex("0492a4dd7a91613c59203721923c94154ac91949264cd8cbb898a9076b90e778ab1621d6ee8e10544b0a0a44747f4522abc2f232ebd12c113ebd57a60d5aecfffb") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 30000000;//coingo
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x0011adb1bb68d0c2294708f7e5a604ea3cca9b55aa952bb5785df45fd54242db");
        consensus.powLimit = uint256S("00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 30; // 30 seconds
        consensus.nPowTargetSpacing = 30; // 30 seconds
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1535108070;//coingo
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1535108169;//coingo

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xf9;
        pchMessageStart[1] = 0xbe;
        pchMessageStart[2] = 0xbb;
        pchMessageStart[3] = 0xdd;
        vAlertPubKey = ParseHex("0408e6f641e4cf520244966a61f30dc849312adc85af5225f9943b378069f251ca1573f69f7aa5d018fc3830bb6392cfe1fd85c4979bbeadc089fb595dd9d2ad3e");
        nDefaultPort = 9988;//coingo
        nMaxTipAge = 100 * 24 * 60 * 60;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1535108070, 567, 0x2000ffff, 1, 0.2 * COIN);//coingo
        //while(!true){// search genesis
        while(!true){
		// search genesis coingo
            static FILE * genesis_file = NULL; if (genesis_file == NULL) {genesis_file = fopen("genesis.info", "w");}
            arith_uint256 hash = UintToArith256(genesis.GetHash());
            arith_uint256 target;
            target.SetCompact(genesis.nBits);
            printf("nonce: %d\npow:%s\nmerkle:%s\n\n"
                                    , genesis.nNonce
                                    , hash.ToString().c_str()
                                    , genesis.hashMerkleRoot.ToString().c_str());
            if (hash < target){
                if(genesis_file != NULL){
                    fprintf(genesis_file, "nonce: %d\npow:%s\nmerkle:%s\n\n"
                        , genesis.nNonce
                        , hash.ToString().c_str()
                        , genesis.hashMerkleRoot.ToString().c_str());
                    fclose(genesis_file); genesis_file = NULL;
                    exit(0);
                }
            }
            genesis.nNonce++;
        }
        assert(genesis.GetHash() == uint256S("0x0011adb1bb68d0c2294708f7e5a604ea3cca9b55aa952bb5785df45fd54242db"));
        assert(genesis.hashMerkleRoot == uint256S("0xc4dd3aecabfdfc68f5e47163eaf54a4669ae57569c66dec2f33c6649eaf7e261"));//coingo
        consensus.hashGenesisBlock = genesis.GetHash();

        vSeeds.push_back(CDNSSeedData("n1.cpucointop.io", "n1.cpucointop.io"));
		vSeeds.push_back(CDNSSeedData("n2.cpucointop.io", "n2.cpucointop.io"));
		vSeeds.push_back(CDNSSeedData("n3.cpucointop.io", "n3.cpucointop.io"));
        vSeeds.push_back(CDNSSeedData("n4.cpucointop.io", "n4.cpucointop.io"));
		vSeeds.push_back(CDNSSeedData("n5.cpucointop.io", "n5.cpucointop.io"));
		
        vSeeds.push_back(CDNSSeedData("n1.cpucoinglobal.io", "n1.cpucoinglobal.io"));
		vSeeds.push_back(CDNSSeedData("n2.cpucoinglobal.io", "n2.cpucoinglobal.io"));
		vSeeds.push_back(CDNSSeedData("n3.cpucoinglobal.io", "n3.cpucoinglobal.io"));
		vSeeds.push_back(CDNSSeedData("n4.cpucoinglobal.io", "n4.cpucoinglobal.io"));
		vSeeds.push_back(CDNSSeedData("n5.cpucoinglobal.io", "n5.cpucoinglobal.io"));
		
        vSeeds.push_back(CDNSSeedData("n1.cpucoinbit.io", "n1.cpucoinbit.io"));
		vSeeds.push_back(CDNSSeedData("n2.cpucoinbit.io", "n2.cpucoinbit.io"));
		vSeeds.push_back(CDNSSeedData("n3.cpucoinbit.io", "n3.cpucoinbit.io"));
		vSeeds.push_back(CDNSSeedData("n4.cpucoinbit.io", "n4.cpucoinbit.io"));
		vSeeds.push_back(CDNSSeedData("n5.cpucoinbit.io", "n5.cpucoinbit.io"));//coingo

        // Ref: https://en.bitcoin.it/wiki/List_of_address_prefixes
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,0); //'1'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,85); //'b'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (        0, uint256S("0x0011adb1bb68d0c2294708f7e5a604ea3cca9b55aa952bb5785df45fd54242db"))//coingo
            (        1, uint256S("0x008fea5f68d2c3792cb392798cc179294765715fec749241f2c6eeef11dc3554"))
            (        2, uint256S("0x00b971b5136008b58646d8f0a1c6c131b066d8020ecf4fc8055ab189b936aa05"))

            ,
            1535108070, // * UNIX timestamp of last checkpoint block coingo
            1,          // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            60000.0     // * estimated number of transactions per day after checkpoint
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 210000;
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x0011adb1bb68d0c2294708f7e5a604ea3cca9b55aa952bb5785df45fd54242db");
        consensus.powLimit = uint256S("00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1456790400; // March 1st, 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1493596800; // May 1st, 2017

        pchMessageStart[0] = 0x0b;
        pchMessageStart[1] = 0x11;
        pchMessageStart[2] = 0x09;
        pchMessageStart[3] = 0x07;
        vAlertPubKey = ParseHex("0408e6f641e4cf520244966a61f30dc849312adc85af5225f9943b378069f251ca1573f69f7aa5d018fc3830bb6392cfe1fd85c4979bbeadc089fb595dd9d2ad3e");
        nDefaultPort = 18333;
        nMaxTipAge = 0x7fffffff;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1535108070, 567, 0x2000ffff, 1, 0.2 * COIN);//coingo
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x0011adb1bb68d0c2294708f7e5a604ea3cca9b55aa952bb5785df45fd54242db"));
        assert(genesis.hashMerkleRoot == uint256S("0xc4dd3aecabfdfc68f5e47163eaf54a4669ae57569c66dec2f33c6649eaf7e261"));

        vFixedSeeds.clear();
        vSeeds.clear();
        //vSeeds.push_back(CDNSSeedData("n1.Cpucoinstatus.info", "n1.Cpucoinstatus.info"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        //vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0x0011adb1bb68d0c2294708f7e5a604ea3cca9b55aa952bb5785df45fd54242db")),
            1337966069,
            1488,
            300
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 0; // BIP34 has not necessarily activated on regtest
        consensus.BIP34Hash = uint256S("0x0011adb1bb68d0c2294708f7e5a604ea3cca9b55aa952bb5785df45fd54242db");
        consensus.powLimit = uint256S("00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nMaxTipAge = 24 * 60 * 60;
        nDefaultPort = 18444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1535108070, 567, 0x2000ffff, 1, 0.2 * COIN);//coingo
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x0011adb1bb68d0c2294708f7e5a604ea3cca9b55aa952bb5785df45fd54242db"));
        assert(genesis.hashMerkleRoot == uint256S("0xc4dd3aecabfdfc68f5e47163eaf54a4669ae57569c66dec2f33c6649eaf7e261"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x0011adb1bb68d0c2294708f7e5a604ea3cca9b55aa952bb5785df45fd54242db")),
            0,
            0,
            0
        };
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}
