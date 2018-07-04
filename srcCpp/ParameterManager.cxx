/*
 * (c) 2005-2018  Copyright, Real-Time Innovations, Inc. All rights reserved.
 * Subject to Eclipse Public License v1.0; see LICENSE.md for details.
 */

#include "ParameterManager.h"

ParameterManager::ParameterManager()
{
}
void ParameterManager::initialize()
{
    Parameter<unsigned long long> *batching = new Parameter<unsigned long long>(0);
    batching->setCommandLineArgument(std::make_pair("-batchsize","<bytes>"));
    batching->setDescription("Size in bytes of batched message, default 8kB.\n(Disabled for LatencyTest mode or if dataLen > 4kB)");
    batching->setType(T_NUMERIC);
    batching->setExtraArgument(YES);
    batching->setRange(0, 63000);
    batching->setGroup(PUB);
    parameterList["batching"] = AnyParameter(batching);

    Parameter<std::string> *flowController = new Parameter<std::string>("default");
    flowController->setCommandLineArgument(std::make_pair("-flowController", "<flow>"));
    flowController->setDescription("In the case asynchronous writer use a specific flow controller.\nThere are several flow controller predefined:\n\t{'default', '1Gbps', '10Gbps'}\nDefault: \"default\" (If using asynchronous).");
    flowController->setType(T_STR);
    flowController->setExtraArgument(YES);
    flowController->addValidStrValue("default");
    flowController->addValidStrValue("1Gbps");
    flowController->addValidStrValue("10Gbps");
    flowController->setGroup(PUB);
    parameterList["flowController"] = AnyParameter(flowController);

    Parameter<std::string> *nic = new Parameter<std::string>("");
    nic->setCommandLineArgument(std::make_pair("-nic","<ipaddr>"));
    nic->setDescription("Use only the nic specified by <ipaddr>.\nIf not specified, use all available interfaces");
    nic->setType(T_STR);
    nic->setExtraArgument(YES);
    nic->setGroup(TRANSPORT);
    parameterList["nic"] = AnyParameter(nic);

    Parameter<bool> * pub = new Parameter<bool>(false);
    pub->setCommandLineArgument(std::make_pair("-pub",""));
    pub->setDescription("Set test to be a publisher");
    pub->setType(T_BOOL);
    pub->setExtraArgument(NO);
    pub->setGroup(GENERAL);
    parameterList["pub"] = AnyParameter(pub);

    ParameterVector<std::string> * peer = new ParameterVector<std::string>();
    peer->setCommandLineArgument(std::make_pair("-peer","<address>"));
    peer->setDescription("Adds a peer to the peer host address list.\nThis argument may be repeated to indicate multiple peers");
    peer->setType(T_VECTOR_STR);
    peer->setExtraArgument(YES);
    peer->setGroup(TRANSPORT);
    parameterList["peer"] = AnyParameter(peer);

    std::vector<unsigned long long> _scanDataLenSizes;
    _scanDataLenSizes.push_back(32);
    _scanDataLenSizes.push_back(64);
    _scanDataLenSizes.push_back(128);
    _scanDataLenSizes.push_back(256);
    _scanDataLenSizes.push_back(512);
    _scanDataLenSizes.push_back(1024);
    _scanDataLenSizes.push_back(2048);
    _scanDataLenSizes.push_back(4096);
    _scanDataLenSizes.push_back(8192);
    _scanDataLenSizes.push_back(16384);
    _scanDataLenSizes.push_back(32768);
    _scanDataLenSizes.push_back(63000);
    ParameterVector<unsigned long long> * scan = new ParameterVector<unsigned long long>(_scanDataLenSizes);
    scan->setCommandLineArgument(std::make_pair("-scan","<size1>:<size2>:...:<sizeN>"));
    scan->setDescription("Run test in scan mode, traversing\na range of sample data sizes from\n[32,63000] or [63001,2147483128] bytes,\nin the case that you are using large data or not.\nThe list of sizes is optional.\nDefault values are '32:64:128:256:512:1024:2048:4096:8192:16384:32768:63000'\nDefault: Not set\n");
    scan->setType(T_VECTOR_NUMERIC);
    scan->setExtraArgument(OPTIONAL);
    scan->setRange(32, 2147483128);
    scan->setParseMethod(SPLIT);
    scan->setGroup(PUB);
    parameterList["scan"] = AnyParameter(scan);

    Parameter<unsigned long long> *unbounded = new Parameter<unsigned long long>(63000);
    unbounded->setCommandLineArgument(std::make_pair("-unbounded","<allocation_threshold>"));
    unbounded->setDescription("Use unbounded Sequences\n<allocation_threshold> is optional, default 63000 Bytes.");
    unbounded->setType(T_NUMERIC);
    unbounded->setExtraArgument(OPTIONAL);
    unbounded->setRange(0, 2147483128);
    unbounded->setGroup(GENERAL);
    parameterList["unbounded"] = AnyParameter(unbounded);

    Parameter<unsigned long long> *sendQueueSize = new Parameter<unsigned long long>(50);
    sendQueueSize->setCommandLineArgument(std::make_pair("-sendQueueSize","<number>"));
    sendQueueSize->setDescription("Sets number of samples (or batches) in send\nqueue. Default: 50");
    sendQueueSize->setType(T_NUMERIC);
    sendQueueSize->setExtraArgument(YES);
    sendQueueSize->setGroup(PUB);
    sendQueueSize->setRange(0, 2147483128);
    parameterList["sendQueueSize"] = AnyParameter(sendQueueSize);

    Parameter<unsigned long long> *domain = new Parameter<unsigned long long>(1);
    domain->setCommandLineArgument(std::make_pair("-domain","<id>"));
    domain->setDescription("RTI DDS Domain. Default: 1");
    domain->setType(T_NUMERIC);
    domain->setExtraArgument(YES);
    domain->setRange(0, 250);
    domain->setGroup(GENERAL);
    parameterList["domain"] = AnyParameter(domain);

    Parameter<std::string> *qosFile = new Parameter<std::string>("perftest_qos_profiles.xml");
    qosFile->setCommandLineArgument(std::make_pair("-qosFile","<filename>"));
    qosFile->setDescription("Name of XML file for DDS Qos profiles.\nDefault: perftest_qos_profiles.xml");
    qosFile->setType(T_STR);
    qosFile->setExtraArgument(YES);
    qosFile->setGroup(GENERAL);
    parameterList["qosFile"] = AnyParameter(qosFile);

    Parameter<std::string> *qosLibrary = new Parameter<std::string>("PerftestQosLibrary");
    qosLibrary->setCommandLineArgument(std::make_pair("-qosLibrary","<lib name>"));
    qosLibrary->setDescription("Name of QoS Library for DDS Qos profiles.\nDefault: PerftestQosLibrary");
    qosLibrary->setType(T_STR);
    qosLibrary->setExtraArgument(YES);
    qosLibrary->setGroup(GENERAL);
    parameterList["qosLibrary"] = AnyParameter(qosLibrary);

    Parameter<bool> *bestEffort = new Parameter<bool>(false);
    bestEffort->setCommandLineArgument(std::make_pair("-bestEffort",""));
    bestEffort->setDescription("Run test in best effort mode. Default: reliable");
    bestEffort->setType(T_BOOL);
    bestEffort->setExtraArgument(NO);
    bestEffort->setGroup(GENERAL);
    parameterList["bestEffort"] = AnyParameter(bestEffort);

    Parameter<bool> *noPositiveAcks = new Parameter<bool>(false);
    noPositiveAcks->setCommandLineArgument(std::make_pair("-noPositiveAcks",""));
    noPositiveAcks->setDescription("Disable use of positive acks in reliable \nprotocol. Default use positive acks");
    noPositiveAcks->setType(T_BOOL);
    noPositiveAcks->setExtraArgument(NO);
    noPositiveAcks->setGroup(GENERAL);
    parameterList["noPositiveAcks"] = AnyParameter(noPositiveAcks);

    ParameterVector<unsigned long long> *cft = new ParameterVector<unsigned long long>();
    cft->setCommandLineArgument(std::make_pair("-cft","<start>:<end>"));
    cft->setDescription("Use a Content Filtered Topic for the Throughput topic in the subscriber side.\nSpecify 2 parameters: <start> and <end> to receive samples with a key in that range.\nSpecify only 1 parameter to receive samples with that exact key.\nDefault: Not set");
    cft->setType(T_VECTOR_NUMERIC);
    cft->setExtraArgument(YES);
    cft->setRange(0, 2147483128);
    cft->setParseMethod(SPLIT);
    cft->setGroup(SUB);
    parameterList["cft"] = AnyParameter(cft);


    ParameterPair<unsigned long long, std::string> *pubRate = new ParameterPair<unsigned long long, std::string>(0,"sleep");
    pubRate->setCommandLineArgument(std::make_pair("-pubRate","<samples/s>:<method>"));
    pubRate->setDescription("Limit the throughput to the specified number\nof samples/s. Default 0 (don't limit)\n[OPTIONAL] Method to control the throughput can be:\n'spin' or 'sleep'\nDefault method: spin\n");
    pubRate->setType(T_PAIR_NUMERIC_STR);
    pubRate->setExtraArgument(YES);
    pubRate->setGroup(PUB);
    pubRate->setRange(0, 10000000);
    pubRate->addValidStrValue("sleep");
    pubRate->addValidStrValue("spin");
    parameterList["pubRate"] = AnyParameter(pubRate);

    Parameter<unsigned long long> *durability = new Parameter<unsigned long long>(0);
    durability->setCommandLineArgument(std::make_pair("-durability","<0|1|2|3>"));
    durability->setDescription("Set durability QOS, 0 - volatile,\n1 - transient local, 2 - transient, \n3 - persistent. Default: 0");
    durability->setType(T_NUMERIC);
    durability->setExtraArgument(YES);
    durability->setGroup(GENERAL);
    durability->setRange(0, 3);
    parameterList["durability"] = AnyParameter(durability);

    Parameter<bool> *dynamicData = new Parameter<bool>(false);
    dynamicData->setCommandLineArgument(std::make_pair("-dynamicData",""));
    dynamicData->setDescription("Makes use of the Dynamic Data APIs instead\nof using the generated types.");
    dynamicData->setType(T_BOOL);
    dynamicData->setExtraArgument(NO);
    dynamicData->setGroup(GENERAL);
    parameterList["dynamicData"] = AnyParameter(dynamicData);

    Parameter<bool> *noDirectCommunication = new Parameter<bool>(false);
    noDirectCommunication->setCommandLineArgument(std::make_pair("-noDirectCommunication",""));
    noDirectCommunication->setDescription("Use brokered mode for persistent durability");
    noDirectCommunication->setType(T_BOOL);
    noDirectCommunication->setExtraArgument(NO);
    noDirectCommunication->setGroup(GENERAL);
    parameterList["noDirectCommunication"] = AnyParameter(noDirectCommunication);

    Parameter<unsigned long long> *waitsetDelayUsec = new Parameter<unsigned long long>(100);
    waitsetDelayUsec->setCommandLineArgument(std::make_pair("-waitsetDelayUsec","<usec>"));
    waitsetDelayUsec->setDescription("UseReadThread related. Allows you to\nprocess incoming data in groups, based on the\ntime rather than individually. It can be used\ncombined with -waitsetEventCount.\nDefault 100 usec");
    waitsetDelayUsec->setType(T_NUMERIC);
    waitsetDelayUsec->setExtraArgument(YES);
    waitsetDelayUsec->setGroup(GENERAL);
    waitsetDelayUsec->setRange(0, ULLONG_MAX);
    parameterList["waitsetDelayUsec"] = AnyParameter(waitsetDelayUsec);

    Parameter<unsigned long long> *waitsetEventCount = new Parameter<unsigned long long>(5);
    waitsetEventCount->setCommandLineArgument(std::make_pair("-waitsetDelayUsec","<count>"));
    waitsetEventCount->setDescription("UseReadThread related. Allows you to\nprocess incoming data in groups, based on the\nnumber of samples rather than individually. It\ncan be used combined with -waitsetDelayUsec.\nDefault 5");
    waitsetEventCount->setType(T_NUMERIC);
    waitsetEventCount->setExtraArgument(YES);
    waitsetEventCount->setGroup(GENERAL);
    waitsetEventCount->setRange(0, ULLONG_MAX);
    parameterList["waitsetEventCount"] = AnyParameter(waitsetEventCount);

    Parameter<bool> *enableAutoThrottle = new Parameter<bool>(false);
    enableAutoThrottle->setCommandLineArgument(std::make_pair("-enableAutoThrottle",""));
    enableAutoThrottle->setDescription("Enables the AutoThrottling feature in the\nthroughput DataWriter (pub)");
    enableAutoThrottle->setType(T_BOOL);
    enableAutoThrottle->setExtraArgument(NO);
    enableAutoThrottle->setGroup(PUB);
    parameterList["enableAutoThrottle"] = AnyParameter(enableAutoThrottle);

    Parameter<bool> *enableTurboMode = new Parameter<bool>(false);
    enableTurboMode->setCommandLineArgument(std::make_pair("-enableTurboMode",""));
    enableTurboMode->setDescription("Enables the TurboMode feature in the\nthroughput DataWriter (pub)");
    enableTurboMode->setType(T_BOOL);
    enableTurboMode->setExtraArgument(NO);
    enableTurboMode->setGroup(PUB);
    parameterList["enableTurboMode"] = AnyParameter(enableTurboMode);

    Parameter<bool> *noXmlQos = new Parameter<bool>(false);
    noXmlQos->setCommandLineArgument(std::make_pair("-noXmlQos",""));
    noXmlQos->setDescription("Skip loading the qos profiles from the xml\nprofile");
    noXmlQos->setType(T_BOOL);
    noXmlQos->setExtraArgument(NO);
    noXmlQos->setGroup(GENERAL);
    parameterList["noXmlQos"] = AnyParameter(noXmlQos);

    Parameter<bool> *asynchronous = new Parameter<bool>(false);
    asynchronous->setCommandLineArgument(std::make_pair("-asynchronous",""));
    asynchronous->setDescription("Use asynchronous writer. \nDefault: Not set");
    asynchronous->setType(T_BOOL);
    asynchronous->setExtraArgument(NO);
    asynchronous->setGroup(PUB);
    parameterList["asynchronous"] = AnyParameter(asynchronous);



  #ifdef RTI_SECURE_PERFTEST
    Parameter<bool> *secureEncryptDiscovery = new Parameter<bool>(false);
    secureEncryptDiscovery->setCommandLineArgument(std::make_pair("-secureEncryptDiscovery",""));
    secureEncryptDiscovery->setDescription("Encrypt discovery traffic");
    secureEncryptDiscovery->setType(T_BOOL);
    secureEncryptDiscovery->setExtraArgument(NO);
    secureEncryptDiscovery->setGroup(SECURE);
    parameterList["secureEncryptDiscovery"] = AnyParameter(secureEncryptDiscovery);

    Parameter<bool> *secureSign = new Parameter<bool>(false);
    secureSign->setCommandLineArgument(std::make_pair("-secureSign",""));
    secureSign->setDescription("Sign (HMAC) discovery and user data");
    secureSign->setType(T_BOOL);
    secureSign->setExtraArgument(NO);
    secureSign->setGroup(SECURE);
    parameterList["secureSign"] = AnyParameter(secureSign);

    Parameter<bool> *secureEncryptData = new Parameter<bool>(false);
    secureEncryptData->setCommandLineArgument(std::make_pair("-secureEncryptData",""));
    secureEncryptData->setDescription("Encrypt topic (user) data");
    secureEncryptData->setType(T_BOOL);
    secureEncryptData->setExtraArgument(NO);
    secureEncryptData->setGroup(SECURE);
    parameterList["secureEncryptData"] = AnyParameter(secureEncryptData);

    Parameter<bool> *secureEncryptSM = new Parameter<bool>(false);
    secureEncryptSM->setCommandLineArgument(std::make_pair("-secureEncryptSM",""));
    secureEncryptSM->setDescription("Encrypt RTPS submessages");
    secureEncryptSM->setType(T_BOOL);
    secureEncryptSM->setExtraArgument(NO);
    secureEncryptSM->setGroup(SECURE);
    parameterList["secureEncryptSM"] = AnyParameter(secureEncryptSM);

    Parameter<std::string> *secureGovernanceFile = new Parameter<std::string>("");
    secureGovernanceFile->setCommandLineArgument(std::make_pair("-secureGovernanceFile","<file>"));
    secureGovernanceFile->setDescription("Governance file. If specified, the authentication,\nsigning, and encryption arguments are ignored. The\ngovernance document configuration will be used instead.\nDefault: built using the secure options.");
    secureGovernanceFile->setType(T_STR);
    secureGovernanceFile->setExtraArgument(YES);
    secureGovernanceFile->setGroup(SECURE);
    parameterList["secureGovernanceFile"] = AnyParameter(secureGovernanceFile);

    Parameter<std::string> *securePermissionsFile = new Parameter<std::string>("");
    securePermissionsFile->setCommandLineArgument(std::make_pair("-securePermissionsFile","<file>"));
    securePermissionsFile->setDescription("Permissions file <optional>.\nDefault: \"./resource/secure/signed_PerftestPermissionsSub.xml\"");
    securePermissionsFile->setType(T_STR);
    securePermissionsFile->setExtraArgument(YES);
    securePermissionsFile->setGroup(SECURE);
    parameterList["securePermissionsFile"] = AnyParameter(securePermissionsFile);

    Parameter<std::string> *secureCertAuthority = new Parameter<std::string>("");
    secureCertAuthority->setCommandLineArgument(std::make_pair("-secureCertAuthority","<file>"));
    secureCertAuthority->setDescription("Certificate authority file <optional>.\nDefault: \"./resource/secure/cacert.pem\"");
    secureCertAuthority->setType(T_STR);
    secureCertAuthority->setExtraArgument(YES);
    secureCertAuthority->setGroup(SECURE);
    parameterList["secureCertAuthority"] = AnyParameter(secureCertAuthority);

    Parameter<std::string> *secureCertFile = new Parameter<std::string>("");
    secureCertFile->setCommandLineArgument(std::make_pair("-secureCertFile","<file>"));
    secureCertFile->setDescription("Certificate file <optional>.\nDefault: \"./resource/secure/sub.pem\"");
    secureCertFile->setType(T_STR);
    secureCertFile->setExtraArgument(YES);
    secureCertFile->setGroup(SECURE);
    parameterList["secureCertFile"] = AnyParameter(secureCertFile);

    Parameter<std::string> *securePrivateKey = new Parameter<std::string>("");
    securePrivateKey->setCommandLineArgument(std::make_pair("-securePrivateKey","<file>"));
    securePrivateKey->setDescription("Private key file <optional>.\nDefault: \"./resource/secure/subkey.pem\"");
    securePrivateKey->setType(T_STR);
    securePrivateKey->setExtraArgument(YES);
    securePrivateKey->setGroup(SECURE);
    parameterList["securePrivateKey"] = AnyParameter(securePrivateKey);
  #endif
}


// Parse the command line parameters and set the value
bool ParameterManager::parse(int argc, char *argv[])
{
    unsigned long long var;
    bool success = true;
    // Copy all arguments into a container of strings
    std::vector<std::string> allArgs(argv, argv + argc);

    std::map<std::string, AnyParameter>::iterator it;
    for (unsigned int i = 1; i < allArgs.size(); i++) {
        for (it = parameterList.begin(); it != parameterList.end(); it++) {
            if (allArgs[i] == it->second.get()->getCommandLineArgument().first) { // TODO check for small string compare
                // NumArguments == 0
                if (it->second.get()->getExtraArgument() == NO) {
                    // Type is T_BOOL
                    if (it->second.get()->getType() == T_BOOL) {
                        (static_cast<Parameter<bool>*>(it->second.get<bool>()))->setValue(true);
                    }
                // NumArguments is 1 or optional
                } else if (it->second.get()->getExtraArgument() > NO) {
                    // Check for error in num of arguments
                    if (i+1 >= allArgs.size() || allArgs[i+1].find("-") == 0) {
                        if (it->second.get()->getExtraArgument() == YES) {
                            fprintf(stderr, "Missing '%s' after '%s'\n",
                                it->second.get()->getCommandLineArgument().second.c_str(),
                                it->second.get()->getCommandLineArgument().first.c_str());
                            return false;
                        } else if (it->second.get()->getExtraArgument() == OPTIONAL) {
                            break;
                        }
                    }
                    ++i;
                    // Type is T_STR
                    if (it->second.get()->getType() == T_STR) {
                        if (!it->second.get()->validateStrRange(allArgs[i])) {
                            success = false;
                        }
                        (static_cast<Parameter<std::string>*>(it->second.get<std::string>()))->setValue(allArgs[i]);
                    }
                    // Type is T_NUMERIC
                    else if (it->second.get()->getType() == T_NUMERIC) {
                        if (sscanf(allArgs[i].c_str(), "%llu", &var) != 1) {
                            fprintf(stderr, "Cannot parse '%s' '%s', invalid input.\n",
                                    it->second.get()->getCommandLineArgument().second.c_str(),
                                    it->second.get()->getCommandLineArgument().first.c_str());
                            success = false;
                        }
                        if (!it->second.get()->validateNumericRange(var)) {
                            success = false;
                        }
                        (static_cast<Parameter<unsigned long long>*>(it->second.get<unsigned long long>()))->setValue(var);
                    }
                    // Type is T_VECTOR_STR
                    else if (it->second.get()->getType() == T_VECTOR_STR) {
                        if (NOSPLIT == ((ParameterVector<std::string>*)it->second.getVector<std::string>())->getParseMethod()) {
                            if (!it->second.get()->validateStrRange(allArgs[i])) {
                                success = false;
                            }
                            (static_cast<ParameterVector<std::string>*>(it->second.getVector<std::string>()))->setValue(allArgs[i]);
                        }
                    }
                    // Type is T_VECTOR_NUMERIC
                    else if (it->second.get()->getType() == T_VECTOR_NUMERIC) {
                        if (SPLIT == ((ParameterVector<unsigned long long>*)it->second.getVector<unsigned long long>())->getParseMethod()) {
                            std::vector<std::string> v = split(allArgs[i]);
                            for (unsigned int j = 0; j < v.size(); j++) {
                                if (sscanf(v[j].c_str(), "%llu", &var) != 1) {
                                    fprintf(stderr, "Cannot parse '%s' '%s', invalid input.\n",
                                            it->second.get()->getCommandLineArgument().second.c_str(),
                                            it->second.get()->getCommandLineArgument().first.c_str());
                                    success = false;
                                }
                                if (!it->second.get()->validateNumericRange(var)) {
                                    success = false;
                                }
                                (static_cast<ParameterVector<unsigned long long>*>(it->second.getVector<unsigned long long>()))->setValue(var);
                            }
                        }
                    }
                    // Type is T_PAIR_NUMERIC_STR
                    else if (it->second.get()->getType() == T_PAIR_NUMERIC_STR) {
                        std::vector<std::string> v = split(allArgs[i]);
                        if (v.size() != 2) {
                            fprintf(stderr, "Missing '%s' after '%s'\n",
                                it->second.get()->getCommandLineArgument().second.c_str(),
                                it->second.get()->getCommandLineArgument().first.c_str());
                            return false;
                        } else {
                            if (sscanf(v[0].c_str(), "%llu", &var) != 1) {
                                fprintf(stderr, "Cannot parse '%s' '%s', invalid input.\n",
                                        it->second.get()->getCommandLineArgument().second.c_str(),
                                        it->second.get()->getCommandLineArgument().first.c_str());
                                success = false;
                            }
                            if (!it->second.get()->validateNumericRange(var)) {
                                success = false;
                            }
                            if (!it->second.get()->validateStrRange(v[1])) {
                                success = false;
                            }
                            (static_cast<ParameterPair<unsigned long long, std::string>*>(it->second.getPair<unsigned long long, std::string>()))->setValue(var, v[1]);
                        }
                    }
                }
                break;
            }
        }
        if (it == parameterList.end()) {
            fprintf(stderr, "Cannot parse '%s', invalid input.\n",
                    allArgs[i].c_str());
            success = false;
        }
    }
    return success;
}

// Get the help message
std::string ParameterManager::displayHelp()
{
    std::map<std::string, AnyParameter>::iterator it;
    std::ostringstream oss;
    std::map<GROUP, std::string> output;
    for (int i = GENERAL; i != RAWTRANSPORT+1; i++) {
        switch (static_cast<GROUP>(i)) {
            case GENERAL:
                output[static_cast<GROUP>(i)] +=
                        "\n\n\t=========================== GENERAL Specific Options ===========================\n\n";
                // TODO center "GENERAL Specific Options" and the rest fill with "="
                break;
            case PUB:
                output[static_cast<GROUP>(i)] +=
                        "\n\n\t=========================== PUBLISHER Specific Options ===========================\n\n";
                break;
            case SUB:
                output[static_cast<GROUP>(i)] +=
                        "\n\n\t=========================== SUBSCRIBER Specific Options ===========================\n\n";
                break;
            case TRANSPORT:
                output[static_cast<GROUP>(i)] +=
                        "\n\n\t=========================== TRANSPORT Specific Options ===========================\n\n";
                break;
          #ifdef RTI_SECURE_PERFTEST
            case SECURE:
                output[static_cast<GROUP>(i)] +=
                        "\n\n\t=========================== SECURE Specific Options ===========================\n\n";
                break;
          #endif
            case RAWTRANSPORT:
                output[static_cast<GROUP>(i)] +=
                        "\n\n\t=========================== RAWTRANSPORT Specific Options ===========================\n\n";
                break;
            default:
                break;
        }
    }

    oss << "/**********************************************************************************************/\n"
        << "Usage:\t perftest_cpp [options]\n"
        << "Where [options] are:\n\n";
    output[GENERAL] +="\t-help                           - Print this usage message and exit\n";

    for (it = parameterList.begin(); it != parameterList.end(); it++) {
        if (!it->second.get()->getInternal()) {
            output[it->second.get()->getGroup()] +=
                    it->second.get()->printCommandLineParameter();
        }
    }
    std::map<GROUP, std::string>::iterator itOutput;
    for (itOutput = output.begin(); itOutput != output.end(); itOutput++) {
        oss << itOutput->second;
    }
    oss << "/**********************************************************************************************/\n";
    return oss.str();
}

// check -help option
bool ParameterManager::checkHelp(int argc, char *argv[])
{
    std::vector<std::string> allArgs(argv, argv + argc);
    for (unsigned int i = 1; i < allArgs.size(); i++) {
        if (allArgs[i] == "-help" || allArgs[i] == "-h") {
            std::cout << displayHelp() <<'\n';
            return true;
        }
    }
    return false;
}

ParameterManager::~ParameterManager()
{
    parameterList.clear();
}

// check if a variable has been set
bool ParameterManager::isSet(std::string parameterKey)
{
    std::map<std::string, AnyParameter>::iterator it;
    it = parameterList.find(parameterKey);
    if (it != parameterList.end()) {
        return parameterList[parameterKey].get()->getIsSet();
    } else {
        return false;
    }
}

std::vector<std::string> ParameterManager::split(std::string str, char delimiter)
{
    std::vector<std::string> v;
    std::size_t current, previous = 0;
    current = str.find_first_of(delimiter);
    while (current != std::string::npos) {
        v.push_back(str.substr(previous, current - previous));
        previous = current + 1;
        current = str.find_first_of(delimiter, previous);
    }
    v.push_back(str.substr(previous, current - previous));
    return v;
}