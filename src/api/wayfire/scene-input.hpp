#pragma once

#include <wayfire/nonstd/wlroots.hpp>
#include <wayfire/geometry.hpp>
#include <wayfire/region.hpp>
#include <optional>
#include <memory>

namespace wf
{
namespace scene
{
class node_t;
using node_ptr = std::shared_ptr<node_t>;
}

/**
 * An interface for scene nodes which interact with the keyboard.
 *
 * Note that by default, nodes do not receive keyboard input. Nodes which wish
 * to do so need to have node_flags::ACTIVE_KEYBOARD set.
 */
class keyboard_interaction_t
{
  public:
    keyboard_interaction_t(const keyboard_interaction_t&) = delete;
    keyboard_interaction_t(keyboard_interaction_t&&) = delete;
    keyboard_interaction_t& operator =(const keyboard_interaction_t&) = delete;
    keyboard_interaction_t& operator =(keyboard_interaction_t&&) = delete;

    /**
     * Handle a keyboard enter event.
     * This means that the node is now focused.
     */
    virtual void handle_keyboard_enter()
    {}

    /**
     * Handle a keyboard leave event.
     * The node is no longer focused.
     */
    virtual void handle_keyboard_leave()
    {}

    /**
     * Handle a keyboard key event.
     *
     * These are received only after the node has received keyboard focus and
     * before it loses it.
     *
     * @return What should happen with the further processing of the event.
     */
    virtual void handle_keyboard_key(wlr_event_keyboard_key event)
    {}

    keyboard_interaction_t() = default;
    virtual ~keyboard_interaction_t()
    {}
};

/**
 * An interface for scene nodes which interact with pointer input.
 *
 * As opposed to keyboard input, all nodes are eligible for receiving pointer
 * and input. As a result, every node may receive motion, button, etc. events.
 * Nodes which do not wish to process events may simply not accept input at
 * any point (as the default accepts_input implementation does).
 */
class pointer_interaction_t
{
  protected:
    pointer_interaction_t(const pointer_interaction_t&) = delete;
    pointer_interaction_t(pointer_interaction_t&&) = delete;
    pointer_interaction_t& operator =(const pointer_interaction_t&) = delete;
    pointer_interaction_t& operator =(pointer_interaction_t&&) = delete;

  public:
    pointer_interaction_t() = default;
    virtual ~pointer_interaction_t() = default;

    /**
     * The pointer entered the node and thus the node gains pointer focus.
     */
    virtual void handle_pointer_enter(wf::pointf_t position)
    {}

    /**
     * Notify a node that it no longer has pointer focus.
     * This event is always sent after a corresponding pointer_enter event.
     */
    virtual void handle_pointer_leave()
    {}

    /**
     * Handle a button press or release event.
     *
     * When a node consumes a button event, core starts an *implicit grab* for it.
     * This has the effect that all subsequent input events are forwarded to that
     * node, until all buttons are released. Thus, a node is guaranteed to always
     * receive matching press and release events.
     *
     * @param pointer_position The position where the pointer is currently at.
     * @param button The wlr event describing the event.
     */
    virtual void handle_pointer_button(
        const wlr_event_pointer_button& event)
    {}

    /**
     * The user moved the pointer.
     *
     * @param pointer_position The new position of the pointer.
     * @param time_ms The time reported by the device when the event happened.
     */
    virtual void handle_pointer_motion(wf::pointf_t pointer_position,
        uint32_t time_ms)
    {}

    /**
     * The user scrolled.
     *
     * @param pointer_position The position where the pointer is currently at.
     * @param event A structure describing the event.
     */
    virtual void handle_pointer_axis(const wlr_event_pointer_axis& event)
    {}
};

/**
 * An interface for scene nodes which interact with touch input.
 */
class touch_interaction_t
{
  public:
    touch_interaction_t() = default;
    virtual ~touch_interaction_t() = default;

    /**
     * The user pressed down with a finger on the node.
     *
     * @param finger_id The id of the finger pressed down (first is 0, then 1,
     *   2, ..). Note that it is possible that the finger 0 is pressed down on
     *   another node, then the current node may start receiving touch down
     *   events beginning with finger 1, 2, ...
     *
     * @param position The coordinates of the finger.
     */
    virtual void handle_touch_down(uint32_t time_ms, int finger_id,
        wf::pointf_t position)
    {}

    /**
     * The user lifted their finger off the node.
     *
     * @param finger_id The id of the finger being lifted. It is guaranteed that
     *   the finger will have been pressed on the node before.
     */
    virtual void handle_touch_up(uint32_t time_ms, int finger_id)
    {}

    /**
     * The user moved their finger without lifting it off.
     */
    virtual void handle_touch_motion(uint32_t time_ms, int finger_id,
        wf::pointf_t position)
    {}
};
}
