// "Enigma in the Wine Cellar" game for Linux.
// Copyright (C) 2005, 2016, 2021 Chris Sterne <chris_sterne@hotmail.com>
// 
// This file is the MapController class implementation.  The MapController
// class executes a block of control code. 
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along
// with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "EnigmaWC.h"
#include "MapController.h"

//*-------------------------------------*
//* Local declarations and definitions. *
//*-------------------------------------*

#define KState ( 1 << 0 )    // Signal bits.

#define KStackSize 8

//*----------------------*
//* Default constructor. *
//*----------------------*

CMapController::CMapController()
{    
  return;
}

//*-------------------------------------------------------------*
//* This method initializes the controller.  This would be done *
//* after all signal and bytecode blocks are loaded.            * 
//*-------------------------------------------------------------*

void CMapController::Initialize()
{
  // Count the number of signal names in the packed array.
  // Each name is terminated by a '\n' character.

  guint16 Total = 0;
  
  for ( guint16 Position = 0;
        Position < iSignalNames.size();
        ++ Position )
  {
    if ( iSignalNames.at( Position ) == '\n' )
      ++ Total;
  }

  // Set the signal state array size to the total number of signals,
  // and initialize all signals to their current state.  
  
  iSignals.resize( Total );
  
  if ( iSignals.size() == Total )
    Run( iCurrentCode );

  return;
}

//*-------------------------------------------------------*
//* This method changes the controller to its saved state *
//* by running the Saved bytecode block.                  *
//*-------------------------------------------------------*

void CMapController::Load()
{
  Run( iSavedCode );
  return;
}

//*---------------------------------------------------------------*
//* This method saves the state of the controller by creating     *
//* a Saved bytecode block from the current state of all signals. *
//*---------------------------------------------------------------*

void CMapController::Save()
{
  CreateSignalCode( iSavedCode );
  return;
}

//*---------------------------------------------------------*
//* This method changes the controller to its restart state *
//* by running the Restart bytecode block.                  *
//*---------------------------------------------------------*

void CMapController::Restart()
{  
  // Return all signals to the FALSE state. 
  
  for ( guint16 Index = 0;
        Index < iSignals.size();
        ++ Index )
  {
    iSignals.at( Index ) = 0;
  }
  
  // Run the Restart code.  Since all signals have been set to FALSE,
  // only those that are to restart as TRUE or in a random state
  // need be set here.
  
  Run( iRestartCode );
  return;
}

//*-----------------------------------------------------------------*
//* This method returns an numerical index to a signal.  This index *
//* corresponds to that used by bytecode signal instructions.       *
//*-----------------------------------------------------------------*
//* aName:  Signal name.                                            *
//* RETURN: Signal index, or G_MAXUINT16 if not found.              *
//*-----------------------------------------------------------------*

guint16 CMapController::GetSignalIndex( const std::string& aName )
{
  std::string::size_type Position = 0;
  std::string::size_type Terminator;
  int Compare   = 1;
  guint16 Index = 0;  

  do
  {
    Terminator = iSignalNames.find( '\n',  Position );
    
    if ( Terminator != std::string::npos )
    {
      // Compare the provided string to one in the string array.
    
      Compare = iSignalNames.compare( Position,
                                      Terminator - Position,
                                      aName );
                                            
      if ( Compare != 0 )
      {
        // The strings do not match.  Move the character position over the
        // string terminator character, and increase the signal index.
    
        Position = Terminator + 1;
        ++ Index;
      }
    }  
  }
  while (( Compare != 0 ) && ( Terminator != std::string::npos ));
  
  // Return the results of the signal name search.

  if ( Compare == 0 )
    return Index;
  else
    return G_MAXUINT16;
}

//*---------------------------------------------------------*
//* This method sets the state of a controller signal, then *
//* runs the Main bytecode if the signal is being changed.  *
//*---------------------------------------------------------*
//* aIndex: Signal index.                                   *
//* aState: State to be set.                                *
//* RETURN: TRUE if signal state was changed.               *
//*---------------------------------------------------------*

gboolean CMapController::SetSignalState( guint16 aIndex, gboolean aState )
{
  // If the signal index will not exceed the signal array, set the signal
  // State bit as selected by the mask.

  gboolean Change = FALSE;

  if ( aIndex < iSignals.size() )
  {
    char Signal    = iSignals.at( aIndex );
    char SignalNew = Signal;
    
    if ( aState )
      SignalNew |= KState;
    else
      SignalNew &= ~KState;
  
    // Run the Main bytecode only if the signal state is being changed.

    Change = ( SignalNew != Signal );

    if ( Change )
    {
      iSignals.at( aIndex ) = SignalNew;
      Run( iMainCode );
    }
  }
    
  return Change;
}

//*-------------------------------------------------------*
//* This method returns the state of a controller signal. *
//*-------------------------------------------------------*
//* aIndex: Signal index.                                 *
//*-------------------------------------------------------*

gboolean CMapController::GetSignalState( guint16 aIndex )
{ 
  // If the signal index will not exceed the signal array, return
  // the signal's State bit as selected by the mask. 
  
  if ( aIndex < iSignals.size() )
    return ( iSignals.at( aIndex ) & KState );
  else
    return FALSE;
}

//*-----------------------------------------------------------------*
//* This method creates a block of bytecode from the signal states. *
//* The code is a sequence of signal store opcodes which will set   *
//* all signals back to their original state when the code is run   *
//* (e.g. '1 > signal').                                            *
//*-----------------------------------------------------------------*
//* aCode: Reference to bytecode block to be filled.                *
//*-----------------------------------------------------------------*

void CMapController::CreateSignalCode( std::string& aCode  )
{
  // Clear old bytecode from the block.
  
  aCode.clear();
  
  // Add new bytecode to the block.
  
  for ( guint16 Index = 0;
        Index < iSignals.size();
        ++ Index )
  {
    // Write signal state opcode.
    
    if ( iSignals.at( Index ) & KState )
      aCode.push_back( (char)EnigmaWC::OpCode::ETrue );
    else
      aCode.push_back( (char)EnigmaWC::OpCode::EFalse );
      
    // Write signal store opcode.
      
    aCode.push_back( (char)EnigmaWC::OpCode::EStore );

    // Write signal opcode.

    aCode.push_back( (guint8)EnigmaWC::OpCode::ESignal );
    aCode.push_back( (char)Index );
  }

  return;
}

//*---------------------------------------*
//* This method runs a block of bytecode. *
//*---------------------------------------*
//* aCode:  Block of bytecode.            *
//*---------------------------------------*

void CMapController::Run( const std::string& aCode )
{  
  gboolean Stack[ KStackSize ];
  guint8 StackPointer;

  guint8 Index;
  guint16 CodePointer;
  EnigmaWC::OpCode OpCode;
  gboolean Register;

  // Execute all bytecode instructions sequentially in one pass.

  StackPointer = 0;
  CodePointer  = 0;

  while ( CodePointer < aCode.size() )
  {
    OpCode = (EnigmaWC::OpCode)aCode.at( CodePointer );
    
    switch( OpCode )
    {      
      case EnigmaWC::OpCode::EAnd:
        // Perform the logical AND of the last two stack entries
        // (if present), then push the result onto the stack.
        
        if ( StackPointer >= 2 )
        {
          -- StackPointer;
          Register = Stack[ StackPointer ];
          -- StackPointer;
          Stack[ StackPointer ] = Register && Stack[ StackPointer ];
          ++ StackPointer;
        }
        
        ++ CodePointer;
        break;
        
      case EnigmaWC::OpCode::EOr:
        // Perform the logical OR of the last two stack entries
        // (if present), then push the result onto the stack.
        
        if ( StackPointer >= 2 )
        {
          -- StackPointer;
          Register = Stack[ StackPointer ];
          -- StackPointer;
          Stack[ StackPointer ] = Register || Stack[ StackPointer ];
          ++ StackPointer;
        }
        
        ++ CodePointer;
        break;
        
      case EnigmaWC::OpCode::ENot:
        // Invert the last stack entry (if present).
      
        if ( StackPointer != 0 )
        {
          -- StackPointer;
          Stack[ StackPointer ] = !Stack[ StackPointer ];
          ++ StackPointer;
        }
      
        ++ CodePointer;
        break;
          
      case EnigmaWC::OpCode::EXor:
        // Perform the logical XOR of the last two stack entries
        // (if present), then push the result onto the stack.
        
        if ( StackPointer != 0 )
        {
          -- StackPointer;
          Register = Stack[ StackPointer ];
          -- StackPointer;
          Stack[ StackPointer ] = !Register != !Stack[ StackPointer ];
          ++ StackPointer;  
        }
        
        ++ CodePointer;
        break;
        
      case EnigmaWC::OpCode::ESignal:
        // Push a signal state onto the stack.

        ++ CodePointer;
        
        if ( CodePointer < aCode.size() )
        {
          if ( StackPointer < KStackSize )
          {
            Index = (guint8)aCode.at( CodePointer );
          
            // Get the signal bit state.
          
            if ( Index < iSignals.size() )
            {                
              Stack[ StackPointer ] = iSignals.at( Index ) & KState;
              ++ StackPointer;
            }
          }
          
          ++ CodePointer;
        }
 
        break;

      case EnigmaWC::OpCode::EStore:
        // Set a signal state to that of the last stack entry.
        
        ++ CodePointer;
        
        if ( CodePointer < aCode.size() )
        {
          // The EStore opcode is followed by an ESignal opcode.
          
          OpCode = (EnigmaWC::OpCode)aCode.at( CodePointer );
          ++ CodePointer;
          
          if ( CodePointer < aCode.size() )
          {
            if  (( OpCode == EnigmaWC::OpCode::ESignal )
              && ( StackPointer != 0 ))
            {
              -- StackPointer;
              Index = (guint8)aCode.at( CodePointer );
          
              if ( Index < iSignals.size() )
              {
                // Update the signal state bit.
              
                if ( Stack[ StackPointer ] )
                  iSignals.at( Index ) |= KState;
                else
                  iSignals.at( Index ) &= ~KState;
              }
            }
            
            ++ CodePointer;
          }
        }
       
        break;
          
      case EnigmaWC::OpCode::EConditional:
        // Run code depending on last state on the stack.
          
        ++ CodePointer;
          
        if ( StackPointer > 0 )
        {
          -- StackPointer;
            
          // Move code pointer to the TRUE condition code.
          // Otherwise, the code pointer is on the FALSE condition code.
            
          if ( Stack[ StackPointer ] )
            CodePointer += 2;
        }
          
        break;

      case EnigmaWC::OpCode::EJump:
        // Perform a relative jump over bytecode.  A relative offset of zero
        // points to the first bytecode following the jump instruction.
          
        ++ CodePointer;
          
        if ( CodePointer < aCode.size() )
          CodePointer += (guint8)aCode.at( CodePointer ) + 1;
          
        break;

      case EnigmaWC::OpCode::EFalse:
        // Push a FALSE state onto the stack.
        
        if ( StackPointer < KStackSize )
        {
          Stack[ StackPointer ] = FALSE;
          ++ StackPointer;
        }
        
        ++ CodePointer;
        break;
        
      case EnigmaWC::OpCode::ETrue:
        // Push a TRUE state onto the stack.
        
        if ( StackPointer < KStackSize )
        {
          Stack[ StackPointer ] = TRUE;
          ++ StackPointer;
        }
        
        ++ CodePointer;
        break;
          
      case EnigmaWC::OpCode::ERandom:
        // Push a random signal state onto the stack.
        
        if ( StackPointer < KStackSize )
        {
          Stack[ StackPointer ] = iRandom.get_bool();
          ++ StackPointer;
        }
          
        ++ CodePointer;
        break;
      
      case EnigmaWC::OpCode::ENone:
        // No operation instruction.
        
        ++ CodePointer;
        break;
      
      default:
        // The OpCode was not recognized.  Set the code pointer to
        // the code block size to prevent further running (the unrecognized
        // instruction will also have an unknown length, so simply skipping
        // over it is not possible).
        
        CodePointer = aCode.size();
        break;       
    }
  }
  
  return;
}
