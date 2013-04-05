using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Windows;

namespace UIHeavyClient
{

    enum GameState {GAME_STATE_NONE, GAME_STATE_EDITION, GAME_STATE_PLAY, GAME_STATE_MAIN_MENU, GAME_STATE_TOURNAMENT_MENU, GAME_STATE_ONLINE_LOBBY}

    // Declaration des prototypes de callbacks
    public delegate bool MessageReceivedCallBack(IntPtr username, IntPtr message, IntPtr groupName);
    public delegate bool EventReceivedCallBack(EventCodes id, IntPtr message);
    public delegate bool EditionEventCallBack(EditionEventCodes pEvent);


    struct CallbackInfos
    {
        public List<CallbackContainer> mCallbackContainer;
        public List<UIElement> mStateElements;
    }


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


        // On garde une reference aux methodes de callback pour eviter que le GC delete tout.
        static CallbackContainer mLocalReferenceToCallbacks;


        public static void Init()
        {
            mLocalReferenceToCallbacks = new CallbackContainer();
            mLocalReferenceToCallbacks.mMessageReceivedCallback=MessageReceivedCallBackManager;
            mLocalReferenceToCallbacks.mEventReceivedCallback=EventReceivedCallBackManager;
            mLocalReferenceToCallbacks.mEditionEventCallback=EditionEventCallBackManager;
            SetEventCallback(mLocalReferenceToCallbacks.mEventReceivedCallback);
            SetEditionEventCallBack(mLocalReferenceToCallbacks.mEditionEventCallback);
            SetMessageCallback(mLocalReferenceToCallbacks.mMessageReceivedCallback);
        }


        // Peut retourner null
        static List<CallbackContainer> getCallbackContainer(GameState pGameState)
        {
            CallbackInfos wCallbackToApply;
            mCallbacks.TryGetValue(pGameState, out wCallbackToApply);
            return wCallbackToApply.mCallbackContainer;
        }

        // Declaration des Callbacks pour le dispatch
        static bool MessageReceivedCallBackManager(IntPtr username, IntPtr message, IntPtr grouName)
        {
            bool wReturnValue = true;
            List<CallbackContainer> wCallbacks = getCallbackContainer(mWantedGameState);
            if(wCallbacks != null)
            {
                foreach(CallbackContainer wContainer in wCallbacks)
                {
                    if(wContainer.mMessageReceivedCallback != null)
                    {
                        wReturnValue = wReturnValue && wContainer.mMessageReceivedCallback(username, message, grouName);
                    }
                }
            }
            return wReturnValue; // Va return false si un des callbacks return false
        }

        static bool EventReceivedCallBackManager(EventCodes id, IntPtr message)
        {
            bool wReturnValue=true;
            List<CallbackContainer> wCallbacks=getCallbackContainer(mWantedGameState);
            if (wCallbacks!=null)
            {
                foreach (CallbackContainer wContainer in wCallbacks)
                {
                    if (wContainer.mEventReceivedCallback!=null)
                    {
                        wReturnValue=wReturnValue && wContainer.mEventReceivedCallback(id, message);
                    }
                }
            }
            return wReturnValue; // Va return false si un des callbacks return false
        }

        static bool EditionEventCallBackManager(EditionEventCodes pEvent)
        {
            bool wReturnValue=true;
            List<CallbackContainer> wCallbacks=getCallbackContainer(mWantedGameState);
            if (wCallbacks!=null)
            {
                foreach (CallbackContainer wContainer in wCallbacks)
                {
                    if (wContainer.mEditionEventCallback!=null)
                    {
                        wReturnValue=wReturnValue && wContainer.mEditionEventCallback(pEvent);
                    }
                }
            }
            return wReturnValue; // Va return false si un des callbacks return false
        }


        // Attibuts
        private static Dictionary<GameState, CallbackInfos> mCallbacks = new Dictionary<GameState, CallbackInfos>();

        private static GameState mCurrentGameState = GameState.GAME_STATE_NONE;
        private static GameState mWantedGameState = GameState.GAME_STATE_NONE;
        private static bool mUnCommittedModification = false;



        // Methode pour initialiser la liste des callbacks
        public static void AddState(GameState pState, List<CallbackContainer> pContainer, List<UIElement> pElements)
        {
            // Check if the state already exist
            CallbackInfos wStateToCheck;
            if (!mCallbacks.TryGetValue(pState, out wStateToCheck))
            {
                // If not create it and insert containers
                wStateToCheck = new CallbackInfos();
                wStateToCheck.mCallbackContainer = pContainer;
                wStateToCheck.mStateElements = pElements;
                mCallbacks.Add(pState, wStateToCheck);
            }
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
            CallbackInfos wCallbackToApply;
            if (mCallbacks.TryGetValue(pState, out wCallbackToApply))
            {
                mWantedGameState = pState;
                mUnCommittedModification = true;

                // Collapsed old state elements
                CallbackInfos wOldCallBack;
                if (mCallbacks.TryGetValue(mCurrentGameState, out wOldCallBack))
                {
                    foreach (UIElement u in wOldCallBack.mStateElements)
                    {
                        u.Visibility = Visibility.Collapsed;
                    }
                }

                // Reveal new state elements
                foreach (UIElement u in wCallbackToApply.mStateElements)
                {
                    u.Visibility = Visibility.Visible;
                }

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
