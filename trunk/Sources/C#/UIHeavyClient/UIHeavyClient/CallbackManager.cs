using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace UIHeavyClient
{

    enum GameState {GAME_STATE_NONE, GAME_STATE_EDITION, GAME_STATE_PLAY, GAME_STATE_LOBBY, GAME_STATE_MAIN_MENU, GAME_STATE_TOURNAMENT_MENU, GAME_STATE_ONLINE_LOBBY}

    // Declaration des prototypes de callbacks
    public delegate bool MessageReceivedCallBack(IntPtr username, IntPtr message);
    public delegate bool EventReceivedCallBack(EventCodes id, IntPtr message);
    public delegate bool EditionEventCallBack(EditionEventCodes pEvent);


    class CallbackContainer 
    {
        public MessageReceivedCallBack     mMessageReceivedCallback = null;
        public EventReceivedCallBack       mEventReceivedCallback   = null;
        public EditionEventCallBack        mEditionEventCallback    = null;
    }


    class CallbackManager
    {
        // Calls on the DLL to set the different callbacks
        [DllImport(@"RazerGame.dll")]
        static extern void SetEventCallback(EventReceivedCallBack callback);

        [DllImport(@"RazerGame.dll")]
        static extern void SetEditionEventCallBack(EditionEventCallBack callback);

        //Callback to received user messages from C++
        [DllImport(@"RazerGame.dll")]
        static extern void SetMessageCallback(MessageReceivedCallBack callback);


        // Attibuts
        private static Dictionary<GameState, CallbackContainer> mCallbacks = new Dictionary<GameState, CallbackContainer>();
        private static GameState mCurrentGameState = GameState.GAME_STATE_NONE;
        private static GameState mWantedGameState = GameState.GAME_STATE_NONE;
        private static bool mUnCommittedModification = false;



        // Methode pour initialiser la liste des callbacks
        public static void AddCallback(GameState pState, CallbackContainer pContainer)
        {
            if(mCallbacks.ContainsKey(pState))
            {
#if DEBUG
                throw new System.Exception("Canot set multiple callbacks for the same GameState");
#endif
            }

            mCallbacks.Add(pState, pContainer);
        }



        ////////////////////////////////////////////////////////////////////////
        /// @fn static bool changeGameMode(GameState pState)
        ///
        /// Methode pour changer le mode des callbacks
        /// 
        /// @param[in] GameState pState : State to go
        ///
        /// @return bool : Success or fail to change
        ////////////////////////////////////////////////////////////////////////
        public static bool ChangeGameMode(GameState pState)
        {
            CallbackContainer wCallbackToApply;
            if (mCallbacks.TryGetValue(pState, out wCallbackToApply))
            {
                SetEventCallback(wCallbackToApply.mEventReceivedCallback);
                SetEditionEventCallBack(wCallbackToApply.mEditionEventCallback);
                SetMessageCallback(wCallbackToApply.mMessageReceivedCallback);
                mWantedGameState = pState;
                mUnCommittedModification = true;
                return true;
            }
            return false;

        }


        ////////////////////////////////////////////////////////////////////////
        /// @fn static void CommitChanges()
        ///
        /// Methode pour appliquer le changement de state
        /// 
        /// @return void
        ////////////////////////////////////////////////////////////////////////
        public static void CommitChanges()
        {
            if(mUnCommittedModification)
            {
                mCurrentGameState = mWantedGameState;
                mUnCommittedModification = false;
            }
            else
            {
#if DEBUG
                throw new System.Exception("Cannot Commit unmodified state");
#endif
            }
        }


        ////////////////////////////////////////////////////////////////////////
        /// @fn static void RevertChanges()
        ///
        /// Methode pour revert et revenir a l'ancien etat. 
        /// Techniquement ne devrait pas lancer d'exception puisqu'on avait 
        /// deja reussi a etre dans l'autre etat.
        /// 
        /// @return void
        ////////////////////////////////////////////////////////////////////////
        public static void RevertChanges()
        {
            if (mUnCommittedModification)
            {
                if (!ChangeGameMode(mCurrentGameState))
                {
#if DEBUG
                    throw new System.Exception("Cannot Revert. Last state threw an error on load");    
#endif
                }
                mUnCommittedModification=false;
            }
            else
            {
#if DEBUG
                throw new System.Exception("Cannot Revert unmodified state");
#endif
            }
        }


    }
}
