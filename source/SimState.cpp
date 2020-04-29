//
//  SimState.cpp
//  x86Simulator
//
//  Created by Rylee Mattingly on 4/25/20.
//  Copyright © 2020 RyleeGM. All rights reserved.
//

#include "SimState.hpp"

using namespace std;

// MARK: - Managing Functions

/**
 This function manages the fetch of instructions.
 */
void SimState::fetch(ifstream *trace){
    
    //Get some info to check if the instruction should be fetched.
    int tempEXE;
    uint64_t tempAddr;
    int tempSize;
    long int firstLoc, secondLoc;
    
    
    //Loop a maximum of fetch width
    for(int i = 0; i < fetchWidth; i++){
        
        //Gather test data
        firstLoc = trace->tellg();
        tempEXE = getNextbool(trace);
        *trace >> hex >> tempAddr;
        tempSize = getNextInt(trace);
        secondLoc = trace->tellg();
        
        //Go back in the file
        trace->seekg(firstLoc - secondLoc, (*trace).cur);
        
        //If the instruction doesn't execute skip it.
        //If the instruction executes and has room in the buffer fetch it.
        //Otherwise, stop fetching.
        if(tempEXE == 0){
            remLine(trace);
        } else if (fetchBufferSize >= tempSize){
            fetchedIns.push(Instruction(trace));
            fetchBufferSize -= tempSize;
        } else {
            return;
        }
    }
}

/**
 This function manages the issue of instruction.
 */
void SimState::issue(){
    
    //Keep track of the instructions commited this cycle
    //-Needed to not break reciprical latency requirements-
    list<tuple<int,int>> instructionsThisCycle;
    
    //Could issue up to the issue width of instructions.
    for(int i = 0; i < issueWidth; i++){
        
        //Return if there are no instructions to consider.
        if(fetchedIns.empty()){
            return;
        }
        
        //Variables for test passage.
        bool initialPass = checkIssueEligibility();
        bool smallRecipPass = true;
        
        /*Check if issue would violate small recip limit*/
        //Iteratr over the small list to check for this cycle.
        for(list<tuple<int,int>>::iterator it = instructionsThisCycle.begin();
            it != instructionsThisCycle.end(); ++it){
            
            //Fail the test if it is there and has a value.
            if(get<0>(*it) == fetchedIns.front().getOpcode()){
                if(get<1>(*it) == 0){
                    smallRecipPass = false;
                }
            }
        }
            
        //Determine final issue eligibility.
        if(smallRecipPass == false || initialPass == false){
            return;
        }
        
        /*Issues the instructions*/
        //Push instruction to the back of the issue list.
        issuedIns.push_back(fetchedIns.front());
        //Pop it off of the fetch queue.
        fetchedIns.pop();
        //Free up space used by that instruction.
        fetchBufferSize += issuedIns.back().getSize();
        //Assign latencies.
        latencyHelper(&issuedIns.back());
        
        //Add recip latency to the correct list.
        float tempRecipLatency = issuedIns.back().getRecipLatency();
        int tempOpcode = issuedIns.back().getOpcode();
        
        //If the recip latency is less equal than 1, add to the cycle count.
        //If the recip is greater than 1, add to the big recip Count.
        if(tempRecipLatency > 1){
            recipCount.push_back(tuple<int, int>(tempOpcode, (int)tempRecipLatency));
        } else {
            /*Check if the instruction type is already in the list.*/
            
            //Bool to determine if the instruction was found during iteration.
            bool insFound = false;
            
            for(list<tuple<int,int>>::iterator it = instructionsThisCycle.begin();
                it != instructionsThisCycle.end(); it++){
                if(get<0>(*it) == tempOpcode){
                    get<0>(*it) -= 1;
                    insFound = true;
                }
            }
            //Finaly decide if we put the instruction in the list.
            if(insFound == false){
                instructionsThisCycle.push_back(tuple<int, int>(tempOpcode,
                                                (int)(1/tempRecipLatency)));
            }
        }
    }
    
}

/**
    This function commits the functions that have completed their work.
 */
void SimState::commit(){
    
    for(list<Instruction>::iterator it = issuedIns.begin(); it != issuedIns.end();){
        //Check if instruction is ready for commit.
        if(it->readyForCommit()){
            //Release functional blocks
            availableLoads += it->getLoadBlocks();
            availableStores += it->getStoreBlocks();
            availableALU += 1;
            
            //Remove from list.
            it = issuedIns.erase(it);
            completeIns += 1;
        } else {
            ++it;
        }
    }
    return;
}

// MARK: - Constructors

/**
 Construct the latency vectors for the simulator and sets setting variables.
 @param latFile     ifstream to file containing latencies. No header.
 @param setFile     ifstream to file containing settings. No header.
 */
SimState::SimState(ifstream *latFile, ifstream *setFile){
    
    //Instantiates the latency vectors to 1587.
    latencyList = vector<int>(1587);
    recipLatencyList = vector<float>(1587);
    
    //Set the 0th element of each list to zeros.
    latencyList[0] = 0;
    recipLatencyList[0] = 0;
    
    cycleCount = 0;
    completeIns = 0;
    
    int indexChecker = 0;
    
    for(int i = 1; i < 1587; i++){
        *latFile >> indexChecker;
        
        //Check that the index is correct.
        if(indexChecker != i){
            cerr << "File Index Error:" << i << indexChecker;
            return;
        }
        
        //Read in the values from the file and store them.
        *latFile >> latencyList[i] >> recipLatencyList[i];
        
        //Go to next line.
        latFile->ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    //Read in settings.
    changeSettings(setFile);
    
    //Set available resource blocks.
    availableLoads = loadBlocks;
    availableStores = storeBlocks;
    availableALU = aluBlocks;
    
    return;
}

// MARK: - Mutators

/**
 Change the settings of the simulator. This will eventually call  Reset()
 @param setFile     ifstream to file containing settings. No header.
 */
void SimState::changeSettings(std::ifstream *setFile){
    
    //Read in settings and clear each comment line.
    *setFile >> fetchWidth;
    setFile->ignore(numeric_limits<streamsize>::max(), '\n');
    
    *setFile >> issueWidth;
    setFile->ignore(numeric_limits<streamsize>::max(), '\n');
    
    *setFile >> loadBlocks;
    setFile->ignore(numeric_limits<streamsize>::max(), '\n');
    
    *setFile >> storeBlocks;
    setFile->ignore(numeric_limits<streamsize>::max(), '\n');
    
    *setFile >> aluBlocks;
    setFile->ignore(numeric_limits<streamsize>::max(), '\n');
    
    *setFile >> isBlockingCache;
    setFile->ignore(numeric_limits<streamsize>::max(), '\n');
    
    *setFile >> fetchBufferSize;
    setFile->ignore(numeric_limits<streamsize>::max(), '\n');
    
    return;
}

// MARK: - Initiation and Termination

/**
 Initiates one CPU cycle and returns on completion.
 @param trace   takes the trace instructions to be fetched from.
 */
void SimState::cycle(ifstream *trace){
    
    //Call for commit.
    commit();
    //Call for issue.
    issue();
    //Call for fetch.
    fetch(trace);
    
    //Decrement Latencies for issued instructions.
    for(list<Instruction>::iterator it = issuedIns.begin();
        it != issuedIns.end(); it++){
        --(*it);
    }
    
    //Decrement any recip latency that prevents continued instructions.
    for(list<tuple<int, int>>::iterator it = recipCount.begin();
        it != recipCount.end();){
        get<1>(*it) -= 1;
        //If it gets to zero on this decrement then remove it.
        if(get<1>(*it) == 0){
            it = recipCount.erase(it);
        } else {
            it++;
        }
    }
    
    //Increment reporting data.
    cycleCount++;
    
}

// MARK: - Issue Helper
/**
 Determines if an instruction is eligable to be issued pendin per cycle eligibility.
 */
bool SimState::checkIssueEligibility(){
    
            //Does the instruction at the head of the queue have a producer
    //in flieght.
    bool producerInPipeline = false;
    bool hasFunctionalBlocksAvailable = true;
    bool bigRecipPass = true;
    bool decision = true;
    
    //For every instruction in the issued list check if it is a producer
    //for the instruction at the head of the queue.
    for(list<Instruction>::iterator it = issuedIns.begin();
        it != issuedIns.end(); ++it){
        
        if(it->isProducer(fetchedIns.front().getRegsRead())){
            producerInPipeline = true;
        }
    }
    
    //Check if functional blocks are available.
    if(availableLoads < fetchedIns.front().getLoadBlocks() &&
       availableStores < fetchedIns.front().getStoreBlocks()){
        
        hasFunctionalBlocksAvailable = false;
    }
    
    /*Check if issue would violate big recip limit.*/
    //Iterate over the big list to check if it contains this instruction.
    for(list<tuple<int,int>>::iterator it = recipCount.begin();
        it != recipCount.end(); ++it){
        
        if(get<0>(*it) == fetchedIns.front().getOpcode()){
            bigRecipPass = false;
        }
    }
    
    //If any of the three tests fail.
    if(producerInPipeline == true || hasFunctionalBlocksAvailable == false ||
       bigRecipPass == false){
        decision = false;
    }
    
    return decision;
}

//MARK: - Latency Helper
/**
 This instruction takes an instruction as a parameter and correctly assigns its latency and reciprical latency.
 @param ins     Instruction for which the latencies should be assined
 */
void SimState::latencyHelper(Instruction *ins){
    
    //Get the latency to check for further analysis.
    int tempLat = latencyList[ins->getOpcode()];
    //Set reciprical latency regardless of further latency calcs.
    ins->setRecipLatency(recipLatencyList[ins->getOpcode()]);
    
    //Check for latency calculation or return.
    if(ins->getRep() > 0){
        ins->setLatency(ins->getRep());
    }else if(tempLat >= 0){
        ins->setLatency(tempLat);
    } else {
        //Switch for variable latencies.
        switch(tempLat){
            case -3 : //XCHG
                if(ins->largestMemR() != 0)
                    ins->setLatency(23);
                else
                    ins->setLatency(2);
                break;
                
            case -6 : //ADD/SUB/ADC/SBB/KADD
                if((ins->largestMemR() + ins->largestMemW()) != 0)
                    ins->setLatency(5);
                else
                    ins->setLatency(1);
                break;
                
            case -7 : //INC/DEC/NOT/NEG
                if((ins->largestMemR() + ins->largestMemW()) != 0)
                    ins->setLatency(6);
                else
                    ins->setLatency(1);
                break;
                
            case -8 : //DIV
                if(ins->largestRegR() == 8)
                    ins->setLatency(61);
                else if(ins->largestRegR() == 4)
                    ins->setLatency(26);
                else
                    ins->setLatency(23);
                break;
                
            case -9 : //IDIV
                if(ins->largestRegR() == 1)
                    ins->setLatency(24);
                else if(ins->largestRegR() == 2)
                    ins->setLatency(23);
                else if(ins->largestRegR() == 4)
                    ins->setLatency(26);
                else
                    ins->setLatency(69);
                break;
                
            case -10 : //AND/OR/XOR/KAND
                if(ins->largestMemR() != 0)
                    ins->setLatency(5);
                else
                    ins->setLatency(1);
                break;
                
            case -13 : //FLD
                if(ins->largestMemR() == 10)
                    ins->setLatency(4);
                else if(ins->largestMemR() != 0)
                    ins->setLatency(3);
                else
                    ins->setLatency(1);
                break;
                
            case -15 : //FSTP
                if(ins->largestMemR() == 10)
                    ins->setLatency(4);
                else if(ins->largestMemR() != 0)
                    ins->setLatency(3);
                else
                    ins->setLatency(1);
                break;
                
            case -21 : //VPMASKMOVD/Q
                if(ins->largestMemR() != 0)
                    ins->setLatency(4);
                else
                    ins->setLatency(14);
                break;
                
            case -23 : //VPBROADCAST W/B
                if(ins->largestMemR() < 16)
                    ins->setLatency(7);
                else
                    ins->setLatency(2);
                break;
                
            case -24 : //VPBroadcast Q/D
                if(ins->largestMemR() < 16)
                    ins->setLatency(4);
                else
                    ins->setLatency(3);
                break;
                
            case -28 : //MOVHPS/MOVHLPS
                if(ins->largestMemR() != 0)
                    ins->setLatency(4);
                else
                    ins->setLatency(3);
                break;
                
            case -33 : //VEXTRACTF128
                if(ins->largestMemR() != 0)
                    ins->setLatency(6);
                else
                    ins->setLatency(3);
                break;
                
            case -34 : //INSERTPS
                if(ins->largestMemR() != 0)
                    ins->setLatency(4);
                else
                    ins->setLatency(1);
                break;
                
            case -35 : //VINSERTF128
                if(ins->largestMemR() != 0)
                    ins->setLatency(5);
                else
                    ins->setLatency(3);
                break;
                
            case -36 : //VMASKMOVPS/D
                if(ins->largestMemR() != 0)
                    ins->setLatency(13);
                else
                    ins->setLatency(3);
                break;
                
            case -40 : //ENTER
                ins->setLatency(87 + 2 * (ins->largestRegR()));
                break;
                
            case -41 : //Repeat
                if(ins->getRep() == 0)
                    ins->setLatency(1);
                else
                    ins->setLatency(ins->getRep());
                break;
        }
    }
    
    return;
}
