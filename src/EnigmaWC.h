/*------------------------------------*
 * Program: Enigma in the Wine Cellar *
 * Version: 5.0 for Linux OS          *
 * File:    EnigmaWC.h                *
 * Date:    October 14, 2016          *
 * Author:  Chris Sterne              *
 *                                    *
 * Common Game map file definitions   *
 * for game and Map Maker.            *
 *------------------------------------*/
 
#ifndef __ENIGMAWC_H__
#define __ENIGMAWC_H__

namespace EnigmaWC
{
  enum class Key        // Key definitions.
  {
    ENone = 0,
    EEnd,               // Introduces the end of keyvalue array.		
    EObject,            // Introduces a simple object.
    EPlayer,            // Introduces a player object.
    EItem,              // Introduces an item object.
    EDescription,       // Introduces a description of the map.
    ESurface,           // Surface on which object is located.
    ERotation,          // Rotation of object on surface.
    EAbove,             // Above location of object.
    ENorth,             // North location of object.
    EEast,              // East location of object.
    EBank,              // Adds high byte onto previous key's value.
    ECategory,          // Category of item.
    EOwner,             // Owner of item.
    EActive,            // Active player or item.
    ELength,            // Length of a data section.
    EData,              // Indicates the start of data section.
    ESaved,             // Saved value.
    ERestart,           // Restart value.
    ECurrent,           // Current value.    
    EController,        // Introduces a map controller.
    ESignal,            // Map controller signal name block.
    ECode,              // Map controller bytecode block.
    ESense,             // Sensor state or signal.
    EState,             // Functional state or signal.
    EVisibility,        // Visibility state or signal.
    EPresence,          // Presence state or signal.
    EOutdoor,           // Player's environment.
    ESelected,          // Selected state of item.
    EUsed,              // Used state of item.
    ETeleporter,        // Introduces a teleporter object.
    EArrival,           // Teleporter arrival value.
    TOTAL
  };

  enum class ID         // Object, Item, Player ID key values.
  {
    ENone = 0,
    EWineCellar,
    EBlockWall,
    EStoneWall,
    ELadder,
    ELadderTop,
    EPlayer,
    EBlocker,
    EMover,
    ETurner,
    EFacer,
    ESurfacer,
    EMushrooms,
    ECorkScrew,
    EWineBottle,
    EBread,
    ECheese,
    EKnife,
    EApple,	
    EWineGlass,
    EPlaceMat,
    EGrapes,
    ESkull,
    EEdgeDrain,
    EAirVent,
    EEasterEgg,
    EFlipper,
    EEdger,
    EStairs,
    EStairsTop,
    ESensor,
    EHandHold,
    EMoss0,
    EMoss90,
    EMoss180,
    EMoss270,
    EMoss360,
    EAirBalloon,
    EPlate,
    EFork,
    ECake,
    EStonePost,
    EStoneButton,
    EWallEyes,
    EOutdoor,
    EIndoor,
    ESkyObjects,
    EArchWay,
    EWoodDoor,
    EWoodWall,
    EPullRing,
    ECubeLock,
    ESphereLock,
    ECubeKey,
    ESphereKey,
    EWaterLayer,
    ELightBeam,
    EEgg,
    EWeb,
    EOrange,
    EFish,
    EWaterBottle,
    EFertilizer,
    ETree,
    ETreeTop,
    ECatWalk,
    ETeleporter,
    EInnerTube,
    EHydrogenBalloon,
    EWeed,
    EFern,
    EVine,
    EWaterLock,
    EAppleLock,
    EOrangeLock,
    EWineLock,
    EPadButton,
    EFence,
    EEarthWall,
    EWater,
    TOTAL
  };

  enum class Direction  // Object surface and rotation key values.
  {
    ENone = 0,
    ENorth,
    ESouth,
    EEast,
    EWest,
    EAbove,
    EBelow,
    ECenter,
    TOTAL
  };

  enum class Category   // Item category key values.
  {
    ENone = 0,
    ERequired,          // Item is required.
    EOptional,          // Item is optional.
    EEasterEgg,         // Item is a secret bonus.
    ESkull,             // Item indicates player is trapped.
    TOTAL
  };

  enum class Language   // Description key language values.
  {
    ENone = 0,
    EEnglish,
    TOTAL
  };

  enum class OpCode     // Controller object opcodes.
  {
    ENone = 0,          // No operation.
    EAnd,               // Logical AND.
    EOr,                // Logical OR.
    ENot,               // Logical NOT.
    EXor,               // Logical XOR.
    ESignal,            // Signal index.
    EStore,             // Store signal value.
    EConditional,       // Conditional block.
    EJump,              // Relative jump.
    EFalse,             // False value.
    ETrue,              // True value.
    ERandom,            // Random (True/False) value.
    TOTAL
  };
};

#endif // __ENIGMAWC_H__