#include"ArmorDetector.h"

void eraseErrorRepeatArmor(vector<ArmorBox> & armors);



void ArmorDetector::matchArmors()
{
    for(int i; i<Lights.size()-1;i++)
    {
        for(int j=i+1;j<Lights.size();j++)
        {
            ArmorBox armor = ArmorBox(Lights[i],Lights[j]);
            if(armor.isSuitableArmor())
            {
                armor.l_index=i;
                armor.r_index=j;
                Armors.emplace_back(armor);
            }
        }
    }
    if(Armors.empty()){
        ArmorState=ARMOR_NOT_FOUND;
        return ;
    }
    else{
        ArmorState=ARMOR_FOUND;
        return ;
    }
}
