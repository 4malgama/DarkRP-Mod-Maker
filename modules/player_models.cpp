#include "../classes.h"

namespace pmv
{
    void append(const char*, const QIcon&);
    void end();
}

namespace modules
{
    namespace pm
    {
        void initPM()
        {
            pmv::append("models/player/alyx.mdl", QIcon("resources/models/preview/alyx.png"));
            pmv::append("models/player/barney.mdl", QIcon("resources/models/preview/barney.png"));
            pmv::append("models/player/breen.mdl", QIcon("resources/models/preview/breen.png"));
            pmv::append("models/player/p2_chell.mdl", QIcon("resources/models/preview/chell.png"));
            pmv::append("models/player/eli.mdl", QIcon("resources/models/preview/eli.png"));
            pmv::append("models/player/gman_high.mdl", QIcon("resources/models/preview/gman.png"));
            pmv::append("models/player/kleiner.mdl", QIcon("resources/models/preview/kleiner.png"));
            pmv::append("models/player/monk.mdl", QIcon("resources/models/preview/monk.png"));
            pmv::append("models/player/mossman.mdl", QIcon("resources/models/preview/mossman.png"));
            pmv::append("models/player/mossman_arctic.mdl", QIcon("resources/models/preview/mossmanarctic.png"));
            pmv::append("models/player/odessa.mdl", QIcon("resources/models/preview/odessa.png"));
            pmv::append("models/player/magnusson.mdl", QIcon("resources/models/preview/magnusson.png"));
            
            pmv::append("models/player/police.mdl", QIcon("resources/models/preview/police.png"));
            pmv::append("models/player/police_fem.mdl", QIcon("resources/models/preview/policefem.png"));
            pmv::append("models/player/combine_soldier.mdl", QIcon("resources/models/preview/combine.png"));
            pmv::append("models/player/combine_super_soldier.mdl", QIcon("resources/models/preview/combineelite.png"));
            pmv::append("models/player/combine_soldier_prisonguard.mdl", QIcon("resources/models/preview/combineprison.png"));
        
            pmv::append("models/player/soldier_stripped.mdl", QIcon("resources/models/preview/stripped.png"));
            pmv::append("models/player/corpse1.mdl", QIcon("resources/models/preview/corpse.png"));
            pmv::append("models/player/charple.mdl", QIcon("resources/models/preview/charple.png"));
            pmv::append("models/player/skeleton.mdl", QIcon("resources/models/preview/skeleton.png"));
            pmv::append("models/player/zombie_classic.mdl", QIcon("resources/models/preview/zombie.png"));
            pmv::append("models/player/zombie_fast.mdl", QIcon("resources/models/preview/zombiefast.png"));
            pmv::append("models/player/zombie_soldier.mdl", QIcon("resources/models/preview/zombine.png"));
            
            pmv::append("models/player/Group01/female_01.mdl", QIcon("resources/models/preview/female01.png"));
            pmv::append("models/player/Group01/female_02.mdl", QIcon("resources/models/preview/female02.png"));
            pmv::append("models/player/Group01/female_03.mdl", QIcon("resources/models/preview/female03.png"));
            pmv::append("models/player/Group01/female_04.mdl", QIcon("resources/models/preview/female04.png"));
            pmv::append("models/player/Group01/female_05.mdl", QIcon("resources/models/preview/female05.png"));
            pmv::append("models/player/Group01/female_06.mdl", QIcon("resources/models/preview/female06.png"));
            
            pmv::append("models/player/Group03/female_01.mdl", QIcon("resources/models/preview/female07.png"));
            pmv::append("models/player/Group03/female_02.mdl", QIcon("resources/models/preview/female08.png"));
            pmv::append("models/player/Group03/female_03.mdl", QIcon("resources/models/preview/female09.png"));
            pmv::append("models/player/Group03/female_04.mdl", QIcon("resources/models/preview/female10.png"));
            pmv::append("models/player/Group03/female_05.mdl", QIcon("resources/models/preview/female11.png"));
            pmv::append("models/player/Group03/female_06.mdl", QIcon("resources/models/preview/female12.png"));
            
            pmv::append("models/player/Group03m/female_01.mdl", QIcon("resources/models/preview/medic10.png"));
            pmv::append("models/player/Group03m/female_02.mdl", QIcon("resources/models/preview/medic11.png"));
            pmv::append("models/player/Group03m/female_03.mdl", QIcon("resources/models/preview/medic12.png"));
            pmv::append("models/player/Group03m/female_04.mdl", QIcon("resources/models/preview/medic13.png"));
            pmv::append("models/player/Group03m/female_05.mdl", QIcon("resources/models/preview/medic14.png"));
            pmv::append("models/player/Group03m/female_06.mdl", QIcon("resources/models/preview/medic15.png"));
            
            pmv::append("models/player/Group01/male_01.mdl", QIcon("resources/models/preview/male01.png"));
            pmv::append("models/player/Group01/male_02.mdl", QIcon("resources/models/preview/male02.png"));
            pmv::append("models/player/Group01/male_03.mdl", QIcon("resources/models/preview/male03.png"));
            pmv::append("models/player/Group01/male_04.mdl", QIcon("resources/models/preview/male04.png"));
            pmv::append("models/player/Group01/male_05.mdl", QIcon("resources/models/preview/male05.png"));
            pmv::append("models/player/Group01/male_06.mdl", QIcon("resources/models/preview/male06.png"));
            pmv::append("models/player/Group01/male_07.mdl", QIcon("resources/models/preview/male07.png"));
            pmv::append("models/player/Group01/male_08.mdl", QIcon("resources/models/preview/male08.png"));
            pmv::append("models/player/Group01/male_09.mdl", QIcon("resources/models/preview/male09.png"));
            
            pmv::append("models/player/Group02/male_02.mdl", QIcon("resources/models/preview/refugee01.png"));
            pmv::append("models/player/Group02/male_04.mdl", QIcon("resources/models/preview/refugee02.png"));
            pmv::append("models/player/Group02/male_06.mdl", QIcon("resources/models/preview/refugee03.png"));
            pmv::append("models/player/Group02/male_08.mdl", QIcon("resources/models/preview/refugee04.png"));
            
            pmv::append("models/player/Group03/male_01.mdl", QIcon("resources/models/preview/male10.png"));
            pmv::append("models/player/Group03/male_02.mdl", QIcon("resources/models/preview/male11.png"));
            pmv::append("models/player/Group03/male_03.mdl", QIcon("resources/models/preview/male12.png"));
            pmv::append("models/player/Group03/male_04.mdl", QIcon("resources/models/preview/male13.png"));
            pmv::append("models/player/Group03/male_05.mdl", QIcon("resources/models/preview/male14.png"));
            pmv::append("models/player/Group03/male_06.mdl", QIcon("resources/models/preview/male15.png"));
            pmv::append("models/player/Group03/male_07.mdl", QIcon("resources/models/preview/male16.png"));
            pmv::append("models/player/Group03/male_08.mdl", QIcon("resources/models/preview/male17.png"));
            pmv::append("models/player/Group03/male_09.mdl", QIcon("resources/models/preview/male18.png"));
            
            pmv::append("models/player/Group03m/male_01.mdl", QIcon("resources/models/preview/medic01.png"));
            pmv::append("models/player/Group03m/male_02.mdl", QIcon("resources/models/preview/medic02.png"));
            pmv::append("models/player/Group03m/male_03.mdl", QIcon("resources/models/preview/medic03.png"));
            pmv::append("models/player/Group03m/male_04.mdl", QIcon("resources/models/preview/medic04.png"));
            pmv::append("models/player/Group03m/male_05.mdl", QIcon("resources/models/preview/medic05.png"));
            pmv::append("models/player/Group03m/male_06.mdl", QIcon("resources/models/preview/medic06.png"));
            pmv::append("models/player/Group03m/male_07.mdl", QIcon("resources/models/preview/medic07.png"));
            pmv::append("models/player/Group03m/male_08.mdl", QIcon("resources/models/preview/medic08.png"));
            pmv::append("models/player/Group03m/male_09.mdl", QIcon("resources/models/preview/medic09.png"));
            
            pmv::append("models/player/hostage/hostage_01.mdl", QIcon("resources/models/preview/hostage01.png"));
            pmv::append("models/player/hostage/hostage_02.mdl", QIcon("resources/models/preview/hostage02.png"));
            pmv::append("models/player/hostage/hostage_03.mdl", QIcon("resources/models/preview/hostage03.png"));
            pmv::append("models/player/hostage/hostage_04.mdl", QIcon("resources/models/preview/hostage04.png"));
            
            pmv::append("models/player/arctic.mdl", QIcon("resources/models/preview/css_arctic.png"));
            pmv::append("models/player/gasmask.mdl", QIcon("resources/models/preview/css_gasmask.png"));
            pmv::append("models/player/guerilla.mdl", QIcon("resources/models/preview/css_guerilla.png"));
            pmv::append("models/player/leet.mdl", QIcon("resources/models/preview/css_leet.png"));
            pmv::append("models/player/phoenix.mdl", QIcon("resources/models/preview/css_phoenix.png"));
            pmv::append("models/player/riot.mdl", QIcon("resources/models/preview/css_riot.png"));
            pmv::append("models/player/swat.mdl", QIcon("resources/models/preview/css_swat.png"));
            pmv::append("models/player/urban.mdl", QIcon("resources/models/preview/css_urban.png"));
            
            pmv::append("models/player/dod_american.mdl", QIcon("resources/models/preview/dod_american.png"));
            pmv::append("models/player/dod_german.mdl", QIcon("resources/models/preview/dod_german.png"));
        }
        
        void close()
        {
            pmv::end();
        }
    }
}
