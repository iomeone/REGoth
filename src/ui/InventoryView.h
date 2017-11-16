#pragma once
#include "View.h"
#include <content/Texture.h>
#include <daedalus/DaedalusGameState.h>

namespace UI
{
class InventoryView : public View
{
public:

    enum class State
    {
        // There are some missing like trade and loot
        Disabled,
        Normal,
        Loot
    };

    InventoryView(Engine::BaseEngine& e);
    ~InventoryView();

    /**
     * Updates/draws the UI-Views
     * @param dt time since last frame
     * @param mstate mouse-state
     */
    void update(double dt, Engine::Input::MouseState& mstate, Render::RenderConfig& config) override;

    State getState() { return m_State; }

    void setState(State state, Handle::EntityHandle other = Handle::EntityHandle::makeInvalidHandle());

    /**
     * @brief enabled Enabled is synonymous with not being hidden
     * @return Whether the inventory is enabled
     */
    bool enabled() { return !m_IsHidden;}

    /**
     * @brief setEnabled Acticates / deactivates input bindings and hides / unhides the view
     * @param enabled Desired enable - state
     */
    void setEnabled(bool enabled);

protected:
    // Input
    int m_DeltaRows;
    int m_DeltaColumns;

    State m_State;
    Handle::EntityHandle m_Other;

    size_t m_SelectedItemSymbol;
    float m_SelectedItemRotation;

    enum ItemDrawFlags
    {
        Selected = 1 << 0,
        Equipped = 1 << 1
    };

    struct ItemDrawState
    {
        // Index into an array of items
        int index;
        // Additional information for rendering the item or it's slot
        int flags; // ItemDrawFlags
    };

    // Holds information about the display and cursor state of an item grid
    struct CursorState
    {
        // Instance symbol of the selected item. If it is not found
        // lastTimeSelectedItemSlot is used.
        size_t selectedItemSymbol;
        // The item slot which was selected last time
        int lastTimeSelectedItemSlot;
        // First row to be displayed. This may increase / decrease as the
        // player is scrolling through the inventory
        int displayedRowsStart;

        // Amount of items visible in the item grid
        int numVisibleItems;
        // Offset of the first item visible
        int offsetItems;
        // Actual index of the selected item
        int itemIndex;

        void reset()
        {
            selectedItemSymbol = -1;
            lastTimeSelectedItemSlot = 0;
            displayedRowsStart = 0;
        }
    };

    // CursorState of the player inventory
    CursorState m_ThisCursorState;
    // CursorState of loot body or container
    CursorState m_OtherCursorState;

    // Consumes m_DeltaRows and m_DeltaColumns to calculate the cursor state
    void calculateCursorState(CursorState &cursorState, float slotSize, Math::float2 inventorySize,
                              const std::vector<Daedalus::GEngineClasses::C_Item> &itemList,
                              const std::vector<int> &indices);

    // Applies the rotations and scale typical for Gothic 1 & 2 inventory view. centerPos is
    // an offset relative to the item's mesh origin, it is the center positions for rotations
    // and scale. If selected is true the item will be scaled up a bit.
    Math::Matrix applyRotationsAndScale(Math::float3 centerPos,
                                        const Daedalus::GEngineClasses::C_Item &itemData,
                                        bool selected);

    // Renders the item into a square with the dimensions of size x size. The item's largest
    // bounding box is scaled to fit that size. Position denotes the upper left corner of that
    // square.
    void drawItem(Render::RenderConfig &config,
                  const Daedalus::GEngineClasses::C_Item &itemData,
                  Math::float2 position, float size, bool selected, float rotationY);

    void drawItemGrid(Render::RenderConfig& config, Math::float2 position,
                      Math::float2 size, UI::EAlign alignment, float slotSize,
                      const std::vector<Daedalus::GEngineClasses::C_Item> &itemList,
                      const std::vector<ItemDrawState> &itemsToDraw);
                      //const std::vector<int> &indices, int selected,
                      //const std::vector<int> &equipped);

    void drawItemInfobox(Render::RenderConfig& config, const Daedalus::GEngineClasses::C_Item &item,
                         Math::float2 position, Math::float2 size);

    static int32_t getPrimarySortValue(const Daedalus::GEngineClasses::C_Item &item);
    bool compareItems(const Daedalus::GEngineClasses::C_Item &l, const Daedalus::GEngineClasses::C_Item &r);

    Handle::TextureHandle m_TexSlot;
    Handle::TextureHandle m_TexSlotHighlighted;
    Handle::TextureHandle m_TexSlotEquipped;
    Handle::TextureHandle m_TexInvBack;

    Engine::ManagedActionBinding m_InputUp;
    Engine::ManagedActionBinding m_InputDown;
    Engine::ManagedActionBinding m_InputLeft;
    Engine::ManagedActionBinding m_InputRight;
    Engine::ManagedActionBinding m_InputUse;
    Engine::ManagedActionBinding m_InputDrop;
    Engine::ManagedActionBinding m_InputAlternate;

    const std::vector<std::string> g2Items = {
            "ItAm_Prot_Fire_01",
            "ItAm_Prot_Edge_01",
            "ItAm_Prot_Point_01",
            "ItAm_Prot_Mage_01",
            "ItAm_Prot_Total_01",
            "ItAm_Dex_01",
            "ItAm_Strg_01",
            "ItAm_Hp_01",
            "ItAm_Mana_01",
            "ItAm_Dex_Strg_01",
            "ItAm_Hp_Mana_01",
            "ItSe_ErzFisch",
            "ItSe_GoldFisch",
            "ItSe_Ringfisch",
            "ItSe_LockpickFisch",
            "ItSe_GoldPocket25",
            "ItSe_GoldPocket50",
            "ItSe_GoldPocket100",
            "ItSe_HannasBeutel",
            "ItLsTorch",
            "ItLsTorchburning",
            "ItLsTorchburned",
            "ItLsTorchFirespit",
            "ItRw_Arrow",
            "ItRw_Bolt",
            "ItRw_Mil_Crossbow",
            "ItRw_Sld_Bow",
            "ItRw_Bow_L_01",
            "ItRw_Bow_L_02",
            "ItRw_Bow_L_03",
            "ItRw_Bow_L_04",
            "ItRw_Bow_M_01",
            "ItRw_Bow_M_02",
            "ItRw_Bow_M_03",
            "ItRw_Bow_M_04",
            "ItRw_Bow_H_01",
            "ItRw_Bow_H_02",
            "ItRw_Bow_H_03",
            "ItRw_Bow_H_04",
            "ItRw_Crossbow_L_01",
            "ItRw_Crossbow_L_02",
            "ItRw_Crossbow_M_01",
            "ItRw_Crossbow_M_02",
            "ItRw_Crossbow_H_01",
            "ItRw_Crossbow_H_02",
            "ItKe_Xardas",
            "ItWr_Canthars_KomproBrief_MIS",
            "ItMw_2h_Rod",
            "ItMi_CoragonsSilber",
            "ItMi_TheklasPaket",
            "ItMi_MariasGoldPlate",
            "ItRi_ValentinosRing",
            "ItKe_Dexter",
            "ItWr_Kraeuterliste",
            "ItWr_ManaRezept",
            "ItWr_Passierschein",
            "ItMi_HerbPaket",
            "ItKe_Storage",
            "ItFo_SmellyFish",
            "ItFo_HalvorFish_MIS",
            "ItWr_HalvorMessage",
            "ItFo_HalvorFish",
            "ItMw_AlriksSword_Mis",
            "ItWr_VatrasMessage",
            "ItWr_VatrasMessage_Open",
            "ItKe_Hotel",
            "ItKe_ThiefGuildKey_MIS",
            "ItKe_ThiefGuildKey_Hotel_MIS",
            "ItKe_Innos_MIS",
            "ItKe_KlosterSchatz",
            "ItKe_KlosterStore",
            "ItKe_KDFPlayer",
            "ItKe_KlosterBibliothek",
            "ItFo_Schafswurst",
            "ItPo_Perm_LittleMana",
            "Holy_Hammer_MIS",
            "ItKe_MagicChest",
            "ItWr_Passage_MIS",
            "ItWr_BanditLetter_MIS",
            "ItWr_Poster_MIS",
            "ItKe_Bandit",
            "ItRw_Bow_L_03_MIS",
            "ItRi_Prot_Point_01_MIS",
            "ItMi_EddasStatue",
            "ItKe_EVT_CRYPT_01",
            "ItKe_EVT_CRYPT_02",
            "ItKe_EVT_CRYPT_03",
            "ITAR_PAL_SKEL",
            "ItKe_Valentino",
            "ItKe_Buerger",
            "ItKe_Richter",
            "ItKe_Salandril",
            "ItKe_PaladinTruhe",
            "ItKe_ThiefTreasure",
            "ItKe_Fingers",
            "ItWr_Schuldenbuch",
            "ItPl_Sagitta_Herb_MIS",
            "ITKE_ORLAN_HOTELZIMMER",
            "ItRw_DragomirsArmbrust_MIS",
            "ItMi_StoneOfKnowlegde_MIS",
            "ItWr_PaladinLetter_MIS",
            "ItWr_LetterForGorn_MIS",
            "ItKe_PrisonKey_MIS",
            "ItKe_OC_Store",
            "ITKE_ErzBaronFlur",
            "ITKE_ErzBaronRaum",
            "ItMi_GornsTreasure_MIS",
            "ItWr_Silvestro_MIS",
            "ItAt_ClawLeader",
            "ItSe_Olav",
            "ItMi_GoldPlate_MIS",
            "ItKe_Pass_MIS",
            "ItKe_Bromor",
            "ITKE_RUNE_MIS",
            "ItWr_Bloody_MIS",
            "ItWr_Pfandbrief_MIS",
            "ItWr_Map_OldWorld_Oremines_MIS",
            "ItWr_Manowar",
            "ItWr_KDWLetter",
            "ItWr_GilbertLetter",
            "ItRi_Tengron",
            "ItMi_InnosEye_MIS",
            "ItMi_InnosEye_Discharged_Mis",
            "ItMi_InnosEye_Broken_Mis",
            "ItWr_PermissionToWearInnosEye_MIS",
            "ItWr_XardasBookForPyrokar_Mis",
            "ItKe_CHEST_SEKOB_XARDASBOOK_MIS",
            "ItWr_CorneliusTagebuch_Mis",
            "ITWR_DementorObsessionBook_MIS",
            "ItWr_PyrokarsObsessionList",
            "ItPo_HealHilda_MIS",
            "ItMw_MalethsGehstock_MIS",
            "ItMi_MalethsBanditGold",
            "ItMi_Moleratlubric_MIS",
            "ItWr_BabosLetter_MIS",
            "ItWr_BabosPinUp_MIS",
            "ItWr_BabosDocs_MIS",
            "ItKe_IgarazChest_Mis",
            "ItWr_Astronomy_Mis",
            "ItPo_HealObsession_MIS",
            "ItSe_Golemchest_Mis",
            "ItWr_ShatteredGolem_MIS",
            "ItWr_DiegosLetter_MIS",
            "ItSe_DiegosTreasure_Mis",
            "ItMi_UltharsHolyWater_Mis",
            "ItWr_MinenAnteil_Mis",
            "ItAm_Prot_BlackEye_Mis",
            "ItMi_KarrasBlessedStone_Mis",
            "ItWr_RichterKomproBrief_MIS",
            "ItWr_MorgahardTip",
            "ItWr_Map_Shrine_MIS",
            "ItWr_VinosKellergeister_Mis",
            "ItAm_Mana_Angar_MIS",
            "ItMW_1H_FerrosSword_Mis",
            "ItMi_KerolothsGeldbeutel_MIS",
            "ItMi_KerolothsGeldbeutelLeer_MIS",
            "ItRw_SengrathsArmbrust_MIS",
            "ItAt_TalbinsLurkerSkin",
            "ItAt_DragonEgg_MIS",
            "ItRi_OrcEliteRing",
            "ItPo_DragonEggDrinkNeoras_MIS",
            "ItWr_Map_Orcelite_MIS",
            "ItWr_Map_Caves_MIS",
            "ItWr_XardasLetterToOpenBook_MIS",
            "ItKe_MonastarySecretLibrary_Mis",
            "ItWr_HallsofIrdorath_Mis",
            "ItWr_HallsofIrdorath_Open_Mis",
            "ItWr_XardasSeamapBook_Mis",
            "ItWr_UseLampIdiot_Mis",
            "ItWr_Seamap_Irdorath",
            "ITWr_ForgedShipLetter_MIS",
            "ITKE_OC_MAINGATE_MIS",
            "ITKE_SHIP_LEVELCHANGE_MIS",
            "ItPo_PotionOfDeath_01_Mis",
            "ItPo_PotionOfDeath_02_Mis",
            "ItAm_AmulettOfDeath_Mis",
            "ItPo_HealRandolph_MIS",
            "ItSe_XardasNotfallBeutel_MIS",
            "ItWr_XardasErmahnungFuerIdioten_MIS",
            "ItWr_Krypta_Garon",
            "ItKe_OrkKnastDI_MIS",
            "ItKe_EVT_UNDEAD_01",
            "ItKe_EVT_UNDEAD_02",
            "ItKe_LastDoorToUndeadDrgDI_MIS",
            "ItWr_LastDoorToUndeadDrgDI_MIS",
            "ItKe_ChestMasterDementor_MIS",
            "ItWr_Rezept_MegaDrink_MIS",
            "ItWr_Diary_BlackNovice_MIS",
            "ItWr_ZugBruecke_MIS",
            "ItMi_PowerEye",
            "ItMw_1h_Vlk_Dagger",
            "ItMw_1H_Mace_L_01",
            "ItMw_1h_Bau_Axe",
            "ItMw_1h_Vlk_Mace",
            "ItMw_1H_Mace_L_03",
            "ItMw_1h_Bau_Mace",
            "ItMw_1h_Vlk_Axe",
            "ItMw_1H_Mace_L_04",
            "ItMw_ShortSword1",
            "ItMw_Nagelknueppel",
            "ItMw_1H_Sword_L_03",
            "ItMw_ShortSword2",
            "ItMw_Sense",
            "ItMw_1h_Vlk_Sword",
            "ItMw_1h_Nov_Mace",
            "ItMw_2h_Bau_Axe",
            "ItMw_2H_Axe_L_01",
            "ItMw_1h_MISC_Sword",
            "ItMw_1h_Misc_Axe",
            "ItMw_2H_Sword_M_01",
            "ItMw_1h_Mil_Sword",
            "ItMw_1h_Sld_Axe",
            "ItMw_1h_Sld_Sword",
            "ItMw_2h_Sld_Axe",
            "ItMw_2h_Sld_Sword",
            "ItMw_1h_Pal_Sword",
            "ItMw_2h_Pal_Sword",
            "ItMw_2H_OrcAxe_01",
            "ItMw_2H_OrcAxe_02",
            "ItMw_2H_OrcAxe_03",
            "ItMw_2H_OrcAxe_04",
            "ItMw_2H_OrcSword_01",
            "ItMw_2H_OrcSword_02",
            "ItMw_ShortSword3",
            "ItMw_Nagelkeule",
            "ItMw_ShortSword4",
            "ItMw_Kriegskeule",
            "ItMw_Richtstab",
            "ItMw_ShortSword5",
            "ItMw_Kriegshammer1",
            "ItMw_Hellebarde",
            "ItMw_Nagelkeule2",
            "ItMw_Schiffsaxt",
            "ItMw_Piratensaebel",
            "ItMw_Schwert",
            "ItMw_1H_Common_01",
            "ItMw_Stabkeule",
            "ItMw_Zweihaender1",
            "ItMw_Steinbrecher",
            "ItMw_Spicker",
            "ItMw_Streitaxt1",
            "ItMw_Schwert1",
            "ItMw_Schwert2",
            "ItMw_Doppelaxt",
            "ItMw_Bartaxt",
            "ItMw_Morgenstern",
            "ItMw_Schwert3",
            "ItMw_Schwert4",
            "ItMw_1H_Special_01",
            "ItMw_2H_Special_01",
            "ItMw_Rapier",
            "ItMw_Rubinklinge",
            "ItMw_Streitkolben",
            "ItMw_Zweihaender2",
            "ItMw_Runenschwert",
            "ItMw_Rabenschnabel",
            "ItMw_Schwert5",
            "ItMw_Inquisitor",
            "ItMw_Streitaxt2",
            "ItMw_Zweihaender3",
            "ItMw_1H_Special_02",
            "ItMw_2H_Special_02",
            "ItMw_ElBastardo",
            "ItMw_Kriegshammer2",
            "ItMw_Meisterdegen",
            "ItMw_Folteraxt",
            "ItMw_Orkschlaechter",
            "ItMw_Zweihaender4",
            "ItMw_Schlachtaxt",
            "ItMw_Krummschwert",
            "ItMw_Barbarenstreitaxt",
            "ItMw_Sturmbringer",
            "ItMw_1H_Special_03",
            "ItMw_2H_Special_03",
            "ItMw_Berserkeraxt",
            "ItMw_Drachenschneide",
            "ItMw_1H_Special_04",
            "ItMw_2H_Special_04",
            "ItMw_1H_Blessed_01",
            "ItMw_1H_Blessed_02",
            "ItMw_1H_Blessed_03",
            "ItMw_2H_Blessed_01",
            "ItMw_2H_Blessed_02",
            "ItMw_2H_Blessed_03",
            "ItMw_1H_Sword_L_01",
            "ItMw_1H_Mace_L_02",
            "ItMw_1H_Axe_L_01",
            "ItMw_1H_Sword_L_02",
            "ItMw_1H_Axe_L_02",
            "ItMw_1H_Sword_L_04",
            "ItMw_1H_Axe_L_03",
            "ItMw_1H_Mace_L_05",
            "ItMw_1H_Sword_L_05",
            "ItMw_1H_Sword_L_06",
            "ItMw_1H_Axe_L_04",
            "ItMw_1H_Mace_L_06",
            "ItMw_1H_Sword_L_07",
            "ItMw_1H_Sword_L_08",
            "ItMw_1H_Axe_L_05",
            "ItMw_1H_Mace_L_07",
            "ItMw_1H_Sword_L_09",
            "ItMw_1H_Sword_L_10",
            "ItMw_1H_Axe_L_06",
            "ItMw_1H_Mace_L_08",
            "ItMw_1H_Mace_L_09",
            "ItMw_1H_Sword_M_01",
            "ItMw_1H_Axe_M_01",
            "ItMw_1H_Mace_M_01",
            "ItMw_1H_Mace_M_02",
            "ItMw_1H_Sword_M_02",
            "ItMw_1H_Axe_M_02",
            "ItMw_1H_Mace_M_03",
            "ItMw_1H_Mace_M_04",
            "ItMw_1H_Sword_M_03",
            "ItMw_1H_Axe_M_03",
            "ItMw_1H_Mace_M_05",
            "ItMw_1H_Mace_M_06",
            "ItMw_1H_Sword_M_04",
            "ItMw_1H_Axe_M_04",
            "ItMw_1H_Mace_M_07",
            "ItMw_1H_Sword_M_05",
            "ItMw_1H_Sword_H_01",
            "ItMw_1H_Axe_H_01",
            "ItMw_1H_Mace_H_01",
            "ItMw_1H_Sword_H_02",
            "ItMw_1H_Sword_H_03",
            "ItMw_1H_Axe_H_02",
            "ItMw_1H_Mace_H_02",
            "ItMw_1H_Sword_H_04",
            "ItMw_1H_Sword_H_05",
            "ItMw_1H_Axe_H_03",
            "ItMw_1H_Mace_H_03",
            "ItMw_2H_Axe_L_02",
            "ItMw_2H_Mace_L_01",
            "ItMw_2H_Sword_L_01",
            "ItMw_2H_Axe_L_03",
            "ItMw_2H_Mace_L_02",
            "ItMw_2H_Mace_L_03",
            "ItMw_2H_Sword_L_02",
            "ItMw_2H_Axe_L_04",
            "ItMw_2H_Mace_L_04",
            "ItMw_2H_Axe_M_01",
            "ItMw_2H_Mace_M_01",
            "ItMw_2H_Mace_M_02",
            "ItMw_2H_Sword_M_02",
            "ItMw_2H_Axe_M_02",
            "ItMw_2H_Mace_M_03",
            "ItMw_2H_Sword_M_03",
            "ItMw_2H_Axe_M_03",
            "ItMw_2H_Mace_M_04",
            "ItMw_2H_Sword_M_04",
            "ItMw_2H_Sword_M_05",
            "ItMw_2H_Axe_M_04",
            "ItMw_2H_Sword_M_06",
            "ItMw_2H_Sword_M_07",
            "ItMw_2H_Axe_M_05",
            "ItMw_2H_Mace_M_05",
            "ItMw_2H_Mace_M_06",
            "ItMw_2H_Sword_M_08",
            "ItMw_2H_Axe_M_06",
            "ItMw_2H_SWORD_M_09",
            "ItMw_2H_Sword_H_01",
            "ItMw_2H_Axe_H_01",
            "ItMw_2H_Sword_H_02",
            "ItMw_2H_Sword_H_03",
            "ItMw_2H_Axe_H_02",
            "ItMw_2H_Sword_H_04",
            "ItMw_2H_Sword_H_05",
            "ItMw_2H_Axe_H_03",
            "ItMw_2H_Sword_H_06",
            "ItMw_2H_Sword_H_07",
            "ItMw_2H_Axe_H_04",
            "ItMw_1H_Blessed_01",
            "ItMw_1H_Blessed_02",
            "ItMw_1H_Blessed_03",
            "ItMw_2H_Blessed_01",
            "ItMw_2H_Blessed_02",
            "ItMw_2H_Blessed_03",
            "ItFo_Apple",
            "ItFo_Cheese",
            "ItFo_Bacon",
            "ItFo_Bread",
            "ItFo_Fish",
            "ItFoMuttonRaw",
            "ItFoMutton",
            "ItFo_Stew",
            "ItFo_XPStew",
            "ItFo_CoragonsBeer",
            "ItFo_FishSoup",
            "ItFo_Sausage",
            "ItFo_Honey",
            "ItFo_Water",
            "ItFo_Beer",
            "ItFo_Booze",
            "ItFo_Wine",
            "ItFo_Milk",
            "ItPl_Weed",
            "ItPl_Beet",
            "ItPl_SwampHerb",
            "ItPl_Mana_Herb_01",
            "ItPl_Mana_Herb_02",
            "ItPl_Mana_Herb_03",
            "ItPl_Health_Herb_01",
            "ItPl_Health_Herb_02",
            "ItPl_Health_Herb_03",
            "ItPl_Dex_Herb_01",
            "ItPl_Strength_Herb_01",
            "ItPl_Speed_Herb_01",
            "ItPl_Mushroom_01",
            "ItPl_Mushroom_02",
            "ItPl_Blueplant",
            "ItPl_Forestberry",
            "ItPl_Planeberry",
            "ItPl_Temp_Herb",
            "ItPl_Perm_Herb",
            "ItRi_Prot_Fire_01",
            "ItRi_Prot_Fire_02",
            "ItRi_Prot_Point_01",
            "ItRi_Prot_Point_02",
            "ItRi_Prot_Edge_01",
            "ItRi_Prot_Edge_02",
            "ItRi_Prot_Mage_01",
            "ItRi_Prot_Mage_02",
            "ItRi_Prot_Total_01",
            "ItRi_Prot_Total_02",
            "ItRi_Dex_01",
            "ItRi_Dex_02",
            "ItRi_Hp_01",
            "ItRi_Hp_02",
            "ItRi_Str_01",
            "ItRi_Str_02",
            "ItRi_Mana_01",
            "ItRi_Mana_02",
            "ItRi_Hp_Mana_01",
            "ItRi_Dex_Strg_01",
            "ITAR_Governor",
            "ITAR_JUDGE",
            "ITAR_SMITH",
            "ITAR_BARKEEPER",
            "ITAR_VLK_L",
            "ITAR_VLK_M",
            "ITAR_VLK_H",
            "ITAR_VlkBabe_L",
            "ITAR_VlkBabe_M",
            "ITAR_VlkBabe_H",
            "ITAR_MIL_L",
            "ITAR_MIL_M",
            "ITAR_PAL_M",
            "ITAR_PAL_H",
            "ITAR_BAU_L",
            "ITAR_BAU_M",
            "ITAR_BauBabe_L",
            "ITAR_BauBabe_M",
            "ITAR_SLD_L",
            "ITAR_SLD_M",
            "ITAR_SLD_H",
            "ITAR_DJG_Crawler",
            "ITAR_DJG_L",
            "ITAR_DJG_M",
            "ITAR_DJG_H",
            "ITAR_DJG_BABE",
            "ITAR_NOV_L",
            "ITAR_KDF_L",
            "ITAR_KDF_H",
            "ITAR_Leather_L",
            "ITAR_BDT_M",
            "ITAR_BDT_H",
            "ITAR_XARDAS",
            "ITAR_LESTER",
            "ITAR_Diego",
            "ITAR_CorAngar",
            "ITAR_Dementor",
            "ITAR_KDW_H",
            "ITAR_Prisoner",
            "ItRu_PalLight",
            "ItRu_PalLightHeal",
            "ItRu_PalMediumHeal",
            "ItRu_PalFullHeal",
            "ItRu_PalHolyBolt",
            "ItRu_PalRepelEvil",
            "ItRu_PalDestroyEvil",
            "ItRu_PalTeleportSecret",
            "ItRu_TeleportSeaport",
            "ItRu_TeleportMonastery",
            "ItRu_TeleportFarm",
            "ItRu_TeleportXardas",
            "ItRu_TeleportPassNW",
            "ItRu_TeleportPassOW",
            "ItRu_TeleportOC",
            "ItRu_TeleportOWDemonTower",
            "ItRu_TeleportTaverne",
            "ItRu_Light",
            "ItRu_FireBolt",
            "ItRu_Zap",
            "ItRu_LightHeal",
            "ItRu_SumGobSkel",
            "ItRu_InstantFireball",
            "ItRu_Icebolt",
            "ItRu_SumWolf",
            "ItRu_Windfist",
            "ItRu_Sleep",
            "ItRu_MediumHeal",
            "ItRu_LightningFlash",
            "ItRu_ChargeFireball",
            "ItRu_SumSkel",
            "ItRu_Fear",
            "ItRu_IceCube",
            "ItRu_ThunderBall",
            "ItRu_SumGol",
            "ItRu_HarmUndead",
            "ItRu_Pyrokinesis",
            "ItRu_Firestorm",
            "ItRu_IceWave",
            "ItRu_SumDemon",
            "ItRu_FullHeal",
            "ItRu_Firerain",
            "ItRu_BreathOfDeath",
            "ItRu_MassDeath",
            "ItRu_MasterOfDisaster",
            "ItRu_ArmyOfDarkness",
            "ItRu_Shrink",
            "ItRu_Deathbolt",
            "ItRu_Deathball",
            "ItRu_Concussionbolt",
            "ItKe_Lockpick",
            "ItKe_Key_01",
            "ItKe_Key_02",
            "ItKe_Key_03",
            "ItKe_City_Tower_01",
            "ItKe_City_Tower_02",
            "ItKe_City_Tower_03",
            "ItKe_City_Tower_04",
            "ItKe_City_Tower_05",
            "ItKe_City_Tower_06",
            "ItSc_PalLight",
            "ItSc_PalLightHeal",
            "ItSc_PalHolyBolt",
            "ItSc_PalMediumHeal",
            "ItSc_PalRepelEvil",
            "ItSc_PalFullHeal",
            "ItSc_PalDestroyEvil",
            "ItSc_Light",
            "ItSc_Firebolt",
            "ItSc_Icebolt",
            "ItSc_LightHeal",
            "ItSc_SumGobSkel",
            "ItSc_InstantFireball",
            "ItSc_Zap",
            "ItSc_SumWolf",
            "ItSc_Windfist",
            "ItSc_Sleep",
            "ItSc_Charm",
            "ItSc_MediumHeal",
            "ItSc_LightningFlash",
            "ItSc_ChargeFireball",
            "ItSc_SumSkel",
            "ItSc_Fear",
            "ItSc_IceCube",
            "ItSc_ThunderBall",
            "ItSc_SumGol",
            "ItSc_HarmUndead",
            "ItSc_Pyrokinesis",
            "ItSc_Firestorm",
            "ItSc_IceWave",
            "ItSc_SumDemon",
            "ItSc_FullHeal",
            "ItSc_Firerain",
            "ItSc_BreathOfDeath",
            "ItSc_MassDeath",
            "ItSc_ArmyOfDarkness",
            "ItSc_Shrink",
            "ItSc_TrfSheep",
            "ItSc_TrfScavenger",
            "ItSc_TrfGiantRat",
            "ItSc_TrfGiantBug",
            "ItSc_TrfWolf",
            "ItSc_TrfWaran",
            "ItSc_TrfSnapper",
            "ItSc_TrfWarg",
            "ItSc_TrfFireWaran",
            "ItSc_TrfLurker",
            "ItSc_TrfShadowbeast",
            "ItSc_TrfDragonSnapper",
            "StandardBrief",
            "StandardBuch",
            "ItWr_Map_NewWorld",
            "ItWr_Map_NewWorld_City",
            "ItWr_Map_OldWorld",
            "ItWr_EinhandBuch",
            "ItWr_ZweihandBuch",
            "ItMi_Stomper",
            "ItMi_RuneBlank",
            "ItMi_Pliers",
            "ItMi_Flask",
            "ItMi_Hammer",
            "ItMi_Scoop",
            "ItMi_Pan",
            "ItMi_PanFull",
            "ItMi_Saw",
            "ItMiSwordraw",
            "ItMiSwordrawhot",
            "ItMiSwordbladehot",
            "ItMiSwordblade",
            "ItMi_Broom",
            "ItMi_Lute",
            "ItMi_Brush",
            "ItMi_Joint",
            "ItMi_Alarmhorn",
            "ItMi_Packet",
            "ItMi_Pocket",
            "ItMi_Nugget",
            "ItMi_Gold",
            "ItMi_OldCoin",
            "ItMi_GoldCandleHolder",
            "ItMi_GoldNecklace",
            "ItMi_SilverRing",
            "ItMi_SilverCup",
            "ItMi_SilverPlate",
            "ItMi_GoldPlate",
            "ItMi_GoldCup",
            "ItMi_BloodCup_MIS",
            "ItMi_GoldRing",
            "ItMi_SilverChalice",
            "ItMi_JeweleryChest",
            "ItMi_GoldChalice",
            "ItMi_GoldChest",
            "ItMi_InnosStatue",
            "ItMi_Sextant",
            "ItMi_SilverCandleHolder",
            "ItMi_SilverNecklace",
            "ItMi_Sulfur",
            "ItMi_Quartz",
            "ItMi_Pitch",
            "ItMi_Rockcrystal",
            "ItMi_Aquamarine",
            "ItMi_HolyWater",
            "ItMi_Coal",
            "ItMi_DarkPearl",
            "ItMi_ApfelTabak",
            "ItMi_PilzTabak",
            "ItMi_DoppelTabak",
            "ItMi_Honigtabak",
            "ItMi_SumpfTabak",
            "ItAt_Addon_BCKopf",
            "ItAt_Meatbugflesh",
            "ItAt_SheepFur",
            "ItAt_WolfFur",
            "ItAt_BugMandibles",
            "ItAt_Claw",
            "ItAt_LurkerClaw",
            "ItAt_Teeth",
            "ItAt_CrawlerMandibles",
            "ItAt_Wing",
            "ItAt_Sting",
            "itat_LurkerSkin",
            "ItAt_WargFur",
            "ItAt_Addon_KeilerFur",
            "ItAt_DrgSnapperHorn",
            "ItAt_CrawlerPlate",
            "ItAt_ShadowFur",
            "ItAt_SharkSkin",
            "ItAt_TrollFur",
            "ItAt_TrollBlackFur",
            "ItAt_WaranFiretongue",
            "ItAt_ShadowHorn",
            "ItAt_SharkTeeth",
            "ItAt_TrollTooth",
            "ItAt_StoneGolemHeart",
            "ItAt_FireGolemHeart",
            "ItAt_IceGolemHeart",
            "ItAt_GoblinBone",
            "ItAt_SkeletonBone",
            "ItAt_DemonHeart",
            "ItAt_UndeadDragonSoulStone",
            "ItAt_IcedragonHeart",
            "ItAt_RockdragonHeart",
            "ItAt_SwampdragonHeart",
            "ItAt_FiredragonHeart",
            "ItAt_DragonBlood",
            "ItAt_DragonScale",
            "ItPo_Mana_01",
            "ItPo_Mana_02",
            "ItPo_Mana_03",
            "ItPo_Health_01",
            "ItPo_Health_02",
            "ItPo_Health_03",
            "ItPo_Perm_STR",
            "ItPo_Perm_DEX",
            "ItPo_Perm_Health",
            "ItPo_Perm_Mana",
            "ItPo_Speed",
            "ItPo_MegaDrink",
            "ItAm_Addon_Franco",
            "ItAm_Addon_Health",
            "ItRi_Addon_Health_01",
            "ItRi_Addon_Health_02",
            "ItAm_Addon_MANA",
            "ItRi_Addon_MANA_01",
            "ItRi_Addon_MANA_02",
            "ItAm_Addon_STR",
            "ItRi_Addon_STR_01",
            "ItRi_Addon_STR_02",
            "ItWr_Addon_BookXP250",
            "ItWr_Addon_BookXP500",
            "ItWr_Addon_BookXP1000",
            "ItWr_Addon_BookLP2",
            "ItWr_Addon_BookLP3",
            "ItWr_Addon_BookLP5",
            "ItWr_Addon_BookLP8",
            "ItBE_Addon_Leather_01",
            "ItBE_Addon_SLD_01",
            "ItBE_Addon_NOV_01",
            "ItBE_Addon_MIL_01",
            "ItBE_Addon_KDF_01",
            "ItBE_Addon_MC",
            "ItBe_Addon_STR_5",
            "ItBe_Addon_STR_10",
            "ItBe_Addon_DEX_5",
            "ItBe_Addon_DEX_10",
            "ItBe_Addon_Prot_EDGE",
            "ItBe_Addon_Prot_Point",
            "ItBe_Addon_Prot_MAGIC",
            "ItBe_Addon_Prot_FIRE",
            "ItBe_Addon_Prot_EdgPoi",
            "ItBe_Addon_Prot_TOTAL",
            "ItWr_SaturasFirstMessage_Addon_Sealed",
            "ItWr_SaturasFirstMessage_Addon",
            "ItMi_Ornament_Addon",
            "ItMi_Ornament_Addon_Vatras",
            "ItWr_Map_NewWorld_Ornaments_Addon",
            "ItWr_Map_NewWorld_Dexter",
            "ItRi_Ranger_Lares_Addon",
            "ItRi_Ranger_Addon",
            "ItRi_LanceRing",
            "ItMi_PortalRing_Addon",
            "ItWr_Martin_MilizEmpfehlung_Addon",
            "ItWr_RavensKidnapperMission_Addon",
            "ItWr_Vatras_KDFEmpfehlung_Addon",
            "ItMi_LostInnosStatue_Daron",
            "ItWr_LuciasLoveLetter_Addon",
            "ItMi_Rake",
            "ItRi_Addon_BanditTrader",
            "ItWr_Addon_BanditTrader",
            "ItWr_Vatras2Saturas_FindRaven",
            "ItWr_Vatras2Saturas_FindRaven_opened",
            "ItAm_Addon_WispDetector",
            "ItFo_Addon_Krokofleisch_Mission",
            "ItRi_Addon_MorgansRing_Mission",
            "ItMi_Focus",
            "ItMi_Addon_Steel_Paket",
            "ItWr_StonePlateCommon_Addon",
            "ItMi_Addon_Stone_01",
            "ItMi_Addon_Stone_05",
            "ItMi_Addon_Stone_03",
            "ItMi_Addon_Stone_04",
            "ItMi_Addon_Stone_02",
            "ItMI_Addon_Kompass_Mis",
            "ItSE_Addon_FrancisChest",
            "ITWR_Addon_FrancisAbrechnung_Mis",
            "ITWR_Addon_GregsLogbuch_Mis",
            "ITKE_Addon_Bloodwyn_01",
            "ITKE_Addon_Heiler",
            "ItMi_TempelTorKey",
            "ItMi_Addon_Bloodwyn_Kopf",
            "ItWR_Addon_TreasureMap",
            "ItMi_Addon_GregsTreasureBottle_MIS",
            "itmi_erolskelch",
            "ItMi_Addon_Shell_01",
            "ItMi_Addon_Shell_02",
            "ItFo_Addon_Shellflesh",
            "ItFo_Addon_Rum",
            "ItFo_Addon_Grog",
            "ItFo_Addon_LousHammer",
            "ItFo_Addon_SchlafHammer",
            "ItFo_Addon_SchnellerHering",
            "ItFo_Addon_Pfeffer_01",
            "ItFo_Addon_FireStew",
            "ItFo_Addon_Meatsoup",
            "ITAR_PIR_L_Addon",
            "ITAR_PIR_M_Addon",
            "ITAR_PIR_H_Addon",
            "ITAR_Thorus_Addon",
            "ITAR_Raven_Addon",
            "ITAR_OreBaron_Addon",
            "ITAR_RANGER_Addon",
            "ITAR_Fake_RANGER",
            "ITAR_KDW_L_Addon",
            "ITAR_Bloodwyn_Addon",
            "ITAR_MayaZombie_Addon",
            "ItAr_FireArmor_Addon",
            "ItRu_Thunderstorm",
            "ItRu_Whirlwind",
            "ItRu_Geyser",
            "ItRu_Waterfist",
            "ItRu_Icelance",
            "ItRu_BeliarsRage",
            "ItRu_SuckEnergy",
            "ItRu_GreenTentacle",
            "ItRu_Swarm",
            "ItRu_Skull",
            "ItRu_SummonZombie",
            "ItRu_SummonGuardian",
            "ITWR_Addon_Runemaking_KDW_CIRC1",
            "ITWR_Addon_Runemaking_KDW_CIRC2",
            "ITWR_Addon_Runemaking_KDW_CIRC3",
            "ITWR_Addon_Runemaking_KDW_CIRC4",
            "ITWR_Addon_Runemaking_KDW_CIRC5",
            "ITWR_Addon_Runemaking_KDW_CIRC6",
            "ITWR_Addon_Runemaking_KDF_CIRC1",
            "ITWR_Addon_Runemaking_KDF_CIRC2",
            "ITWR_Addon_Runemaking_KDF_CIRC3",
            "ITWR_Addon_Runemaking_KDF_CIRC4",
            "ITWR_Addon_Runemaking_KDF_CIRC5",
            "ITWR_Addon_Runemaking_KDF_CIRC6",
            "ITKE_PORTALTEMPELWALKTHROUGH_ADDON",
            "ITKE_Greg_ADDON_MIS",
            "ITKE_Addon_Tavern_01",
            "ITKE_Addon_Esteban_01",
            "ITKE_ORLAN_TELEPORTSTATION",
            "ITKE_CANYONLIBRARY_HIERARCHY_BOOKS_ADDON",
            "ITKE_ADDON_BUDDLER_01",
            "ITKE_ADDON_SKINNER",
            "ITKE_Addon_Thorus",
            "ItSc_Geyser",
            "ItSc_Icelance",
            "ItSc_Waterfist",
            "ItSc_Thunderstorm",
            "ItSc_Whirlwind",
            "ITWr_Addon_Hinweis_02",
            "ITWr_Addon_Health_04",
            "ITWr_Addon_Mana_04",
            "ITWr_Addon_Hinweis_01",
            "ITWr_Addon_William_01",
            "ITWr_Addon_MCELIXIER_01",
            "ITWr_Addon_Pirates_01",
            "ITWr_Addon_Joint_01",
            "ITWr_Addon_Lou_Rezept",
            "ITWr_Addon_Lou_Rezept2",
            "ITWr_Addon_Piratentod",
            "Fakescroll_Addon",
            "ItWr_Addon_AxtAnleitung",
            "ItWr_Addon_SUMMONANCIENTGHOST",
            "ItWr_Map_AddonWorld",
            "ItMi_GoldNugget_Addon",
            "ItMi_Addon_WhitePearl",
            "ItMi_Addon_Joint_01",
            "ItMi_BaltramPaket",
            "ItMi_Packet_Baltram4Skip_Addon",
            "ItMi_BromorsGeld_Addon",
            "ItSe_ADDON_CavalornsBeutel",
            "ItMi_Skull",
            "ItMi_IECello",
            "ItMi_IECelloBow",
            "ItMi_IEDrum",
            "ItMi_IEDrumScheit",
            "ItMi_IEDrumStick",
            "ItMi_IEDudelBlau",
            "ItMi_IEDudelGelb",
            "ItMi_IEHarfe",
            "ItMi_IELaute",
            "ItMi_Addon_Lennar_Paket",
            "ItMi_Zeitspalt_Addon",
            "ItWr_StrStonePlate1_Addon",
            "ItWr_StrStonePlate2_Addon",
            "ItWr_StrStonePlate3_Addon",
            "ItWr_DexStonePlate1_Addon",
            "ItWr_DexStonePlate2_Addon",
            "ItWr_DexStonePlate3_Addon",
            "ItWr_HitPointStonePlate1_Addon",
            "ItWr_HitPointStonePlate2_Addon",
            "ItWr_HitPointStonePlate3_Addon",
            "ItWr_ManaStonePlate1_Addon",
            "ItWr_ManaStonePlate2_Addon",
            "ItWr_ManaStonePlate3_Addon",
            "ItWr_OneHStonePlate1_Addon",
            "ItWr_OneHStonePlate2_Addon",
            "ItWr_OneHStonePlate3_Addon",
            "ItWr_TwoHStonePlate1_Addon",
            "ItWr_TwoHStonePlate2_Addon",
            "ItWr_TwoHStonePlate3_Addon",
            "ItWr_BowStonePlate1_Addon",
            "ItWr_BowStonePlate2_Addon",
            "ItWr_BowStonePlate3_Addon",
            "ItWr_CrsBowStonePlate1_Addon",
            "ItWr_CrsBowStonePlate2_Addon",
            "ItWr_CrsBowStonePlate3_Addon",
            "ItPo_Addon_Geist_01",
            "ItPo_Addon_Geist_02",
            "ItPo_Health_Addon_04",
            "ItPo_Mana_Addon_04",
            "ItMW_Addon_Knife01",
            "ItMW_Addon_Stab01",
            "ItMW_Addon_Stab02",
            "ItMW_Addon_Stab03",
            "ItMW_Addon_Stab04",
            "ItMW_Addon_Stab05",
            "ItMW_Addon_Hacker_1h_01",
            "ItMW_Addon_Hacker_1h_02",
            "ItMW_Addon_Hacker_2h_01",
            "ItMW_Addon_Hacker_2h_02",
            "ItMW_Addon_Keule_1h_01",
            "ItMW_Addon_Keule_2h_01",
            "ItMw_FrancisDagger_Mis",
            "ItMw_RangerStaff_Addon",
            "ItMw_Addon_PIR2hAxe",
            "ItMw_Addon_PIR2hSword",
            "ItMw_Addon_PIR1hAxe",
            "ItMw_Addon_PIR1hSword",
            "ItMw_Addon_BanditTrader",
            "ItMw_Addon_Betty",
            "ItRw_Addon_MagicArrow",
            "ItRw_Addon_FireArrow",
            "ItRw_Addon_MagicBow",
            "ItRw_Addon_FireBow",
            "ItRw_Addon_MagicBolt",
            "ItRw_Addon_MagicCrossbow"
    };
};

} // namespace UI
