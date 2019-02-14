option(PlayingState) {
    initial_state(launch) {
        transition{

            switch (theRobotInfo.number) {
            case 1:
                goto striker;
            case 2:
                goto supporter1;
            case 3:
                goto supporter2;
            case 4:
                goto defender;
            case 5:
                goto keeper;
            default:;
            }
        }
        action{

        }
    }
    state(striker) {
        action{
            StrikerFirst();
        }
    }
    state(supporter1) {
        action{
            SupporterFirst_1();
        }
    }
    state(supporter2) {
        action{
            SupporterFirst_2();
        }
    }
    state(defender) {
        action{
            Defender();
        }
    }
    state(keeper) {
        action{
            Keeper();
        }
    }
}