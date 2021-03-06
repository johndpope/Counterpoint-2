#include "counterpoint.c"

#define NOTES 11

void makeCounterpoint(){
    
    // -999 is code for NULL

    // root = 1
    int cantusFirmus[NOTES] = {1, 3, 2, 1, 4, 3, 5, 4, 3, 2, 1};

    printf("\ncantus firmus:\n");
    for(int i = 0; i < 11; i++)
        printf(" %d  ",cantusFirmus[i]);
    printf("\n\n");

    // root = 0
    for(int i = 0; i < NOTES; i++)
        cantusFirmus[i]--;
    
    
    
    // 0:DO  1:RE  2:MI  3:FA  4:SOL  5:LA  6:TI

    int consonantHarmonies[NOTES*8]; // four up, four down, for each note
    for(int i = 0; i < NOTES; i++){
        // four above
        consonantHarmonies[i*8+0] = cantusFirmus[i] + 3 - 1;
        consonantHarmonies[i*8+1] = cantusFirmus[i] + 5 - 1;
        consonantHarmonies[i*8+2] = cantusFirmus[i] + 6 - 1;
        consonantHarmonies[i*8+3] = cantusFirmus[i] + 8 - 1;
        // four below
        consonantHarmonies[i*8+4] = cantusFirmus[i] - 3 + 1;
        consonantHarmonies[i*8+5] = cantusFirmus[i] - 5 + 1;
        consonantHarmonies[i*8+6] = cantusFirmus[i] - 6 + 1;
        consonantHarmonies[i*8+7] = cantusFirmus[i] - 8 + 1;
    }
    
    
    
    printf("consonant harmonies:\n");
    for(int j = 0; j < 8; j++){
        for(int i = 0; i < NOTES; i++){
            if(consonantHarmonies[i*8+j] == -999)
                printf("     ");
            else if(consonantHarmonies[i*8+j] < 9 && consonantHarmonies[i*8+j] >= 0)
                printf(" %d  ", consonantHarmonies[i*8+j] + 1);
            else if(consonantHarmonies[i*8+j] < -9)
                printf("%d ", consonantHarmonies[i*8+j] - 1);
            else if(consonantHarmonies[i*8+j] < 0)
                printf("%d  ", consonantHarmonies[i*8+j] - 1);
            else
                printf(" %d ", consonantHarmonies[i*8+j] + 1);
            
        }
        printf("\n");
    }
    printf("\n");

//    now build the idea of the interval step
    
    interval intervals[NOTES-1];
    for(int i = 0; i < NOTES-1; i++){
        //oblique
        if(cantusFirmus[i] == cantusFirmus[i+1])
            intervals[i].d = 0;
        // upward
        else if(cantusFirmus[i] < cantusFirmus[i+1])
            intervals[i].d = 1;
        // down
        else if(cantusFirmus[i] > cantusFirmus[i+1])
            intervals[i].d = -1;
    }
    
    
    
    int counterPoint[NOTES];
    counterPoint[0] = 5 - 1; // start on fifth
    
    for(int i = 1; i < NOTES; i++){
        short motion = intervals[i-1].d;
        
        // find a next note
        int possibleNextNotes[8];
        int bestNoteIndex = -1;
        int finalAnswer = -1;
        
        for(int j = 0; j < 8; j++){
            if(finalAnswer == -1){
                possibleNextNotes[j] = consonantHarmonies[i*8+j];
                short attemptedMotion;
                // will be oblique motion
                if(counterPoint[i-1] == possibleNextNotes[j])
                    attemptedMotion = 0;
                // will be downward
                else if(counterPoint[i-1] > possibleNextNotes[j])
                    attemptedMotion = -1;
                // will be upward
                else if(counterPoint[i-1] < possibleNextNotes[j])
                    attemptedMotion = 1;
                
                if(attemptedMotion == 0)
                    bestNoteIndex = finalAnswer = j;
                
                if((motion == -1 && attemptedMotion == -1) || (motion == 1 && attemptedMotion == 1)){
                    // this thing trying to to direct motion
                    // check if we are trying for a perfect interval
                    int interval1 = abs(cantusFirmus[i] - possibleNextNotes[j]);
                    int interval2 = abs(possibleNextNotes[j] - cantusFirmus[i]);
                    
                    if(cantusFirmus[i] - possibleNextNotes[j] == 0 ||
                       possibleNextNotes[j] == 4 ||
                       possibleNextNotes[j] == 7 ||
                       possibleNextNotes[j] == -4 ||
                       possibleNextNotes[j] == -7  )
                    {
                        // direct motion into a perfect consonance
                        // this is not allowed
                        possibleNextNotes[j] = -999;
                    }
                }
                if(bestNoteIndex == -1) bestNoteIndex = j;
                else if(possibleNextNotes[j] != -999){
                    if( abs(cantusFirmus[i] - possibleNextNotes[j]) < abs(cantusFirmus[i] - possibleNextNotes[bestNoteIndex]) )
                        bestNoteIndex = j;
                }
            }
            counterPoint[i] = possibleNextNotes[bestNoteIndex];
        }
    }
    
    
    printf("counterpoint:\n");
    for(int i = 0; i < NOTES; i++){
        if(counterPoint[i] == -999)
            printf("     ");
        else if(counterPoint[i] < 10 && counterPoint[i] >= 0)
            printf(" %d  ", counterPoint[i] + 1);
        else if(counterPoint[i] < -9)
            printf("%d ", counterPoint[i] - 1);
        else if(counterPoint[i] < 0)
            printf("%d  ", counterPoint[i] - 1);
        else
            printf(" %d ", counterPoint[i] + 1);
    }
    printf("\n\n\n");

    
    ///////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////
    
    step steps[10];
    for(int i = 0; i < 10; i++){
        steps[i].fromNote.pitch = cantusFirmus[i];
        steps[i].toNote.pitch = cantusFirmus[i+1];
        if(steps[i].fromNote.pitch == 1 || steps[i].fromNote.pitch == 5 || steps[i].fromNote.pitch == 8){
            steps[i].fromNote.consonance = 1;
            steps[i].fromNote.perfect = 1;
        }
        else if(steps[i].fromNote.pitch == 3 || steps[i].fromNote.pitch == 6){
            steps[i].fromNote.consonance = 1;
            steps[i].fromNote.perfect = 0;
        }
        
        if(steps[i].toNote.pitch == 1 || steps[i].toNote.pitch == 5 || steps[i].toNote.pitch == 8){
            steps[i].toNote.consonance = 1;
            steps[i].toNote.perfect = 1;
        }
        else if(steps[i].toNote.pitch == 3 || steps[i].toNote.pitch == 6){
            steps[i].toNote.consonance = 1;
            steps[i].toNote.perfect = 0;
        }

        if(cantusFirmus[i] < cantusFirmus[i+1]){
            steps[i].direction = 1;
            steps[i].interval = cantusFirmus[i+1] - cantusFirmus[i];
        }
        else{
            steps[i].direction = 0;
            steps[i].interval = cantusFirmus[i] - cantusFirmus[i+1];
        }
        
    }
//    int counterPoint[11];
    counterPoint[0] = 5;
    for(int i = 1; i < 11; i++){
        if(steps[i-1].toNote.consonance && steps[i-1].toNote.perfect){
            // contrary or oblique motion only
            if(steps[i-1].direction){ // CF goes up
                if(arc4random()%2 == 0)
                    counterPoint[i] = counterPoint[i-1] - 1;  // CP goes down, contrary
                else
                    counterPoint[i] = counterPoint[i-1];      // CP stays same, oblique
            }
            if(!steps[i-1].direction){ // CF goes down
                if(arc4random()%2 == 0)
                    counterPoint[i] = counterPoint[i-1] + 1;  // CP goes up, contrary
                else
                    counterPoint[i] = counterPoint[i-1];      // CP stays same, oblique
            }
        }
        else {
            // any 3 movements permitted
            if(arc4random()%3 == 0)
                counterPoint[i] = counterPoint[i-1] + 1;  // CP goes up
            else if(arc4random()%2 == 0)
                counterPoint[i] = counterPoint[i-1] - 1;  // CP goes down
            else
                counterPoint[i] = counterPoint[i-1];      // CP stays same, oblique
        }
        
        if(steps[i].fromNote.consonance && steps[i].toNote.consonance){
            
        }
        else if(!steps[i].fromNote.consonance && steps[i].toNote.consonance){
            
        }
        else if(steps[i].fromNote.consonance && !steps[i].toNote.consonance){
            
        }
        else if(!steps[i].fromNote.consonance && !steps[i].toNote.consonance){
            
        }
    }
    printf("  ");
    for(int i = 0; i < 10; i++){
        if(steps[i].direction) printf("+");
        else                   printf("-");
        printf("%d ",steps[i].interval);
    }
    printf("\ncantus firmus:\n");
    for(int i = 0; i < 11; i++)
        printf(" %d ",cantusFirmus[i]);
    printf("\ncounterpoint:\n");
    for(int i = 0; i < 11; i++)
        printf(" %d ",counterPoint[i]);
    printf("\n\n");

}