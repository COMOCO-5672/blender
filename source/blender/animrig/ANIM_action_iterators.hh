/* SPDX-FileCopyrightText: 2024 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

/** \file
 * \ingroup animrig
 *
 * \brief Functionality to iterate an Action in various ways.
 */

#pragma once

#include <cstdint>

#include "BLI_vector.hh"
#include "DNA_action_types.h"

struct FCurve;
namespace blender::animrig {
class Action;
class Layer;
class Strip;
class ChannelBag;
}  // namespace blender::animrig

namespace blender::animrig {

using slot_handle_t = decltype(::ActionSlot::handle);

/**
 * Iterates over all FCurves of the given slot handle in the Action and executes the callback on
 * it. Works on layered and legacy actions. When the action is legacy, the slot handle will be
 * ignored.
 *
 * \note Use lambdas to have access to specific data in the callback.
 */
void action_foreach_fcurve(Action &action,
                           slot_handle_t handle,
                           FunctionRef<void(FCurve &fcurve)> callback);

/**
 * Call the given callback for each Action + Slot that this ID uses.
 *
 * The following cases are visited:
 *   - Direct Action+Slot assignment.
 *   - NLA strips.
 *   - Action Constraints, both on Object and Pose Bone level.
 *
 * \param callback The function to call for each Action+Slot used. Even when there is no slot
 * assigned, this function will be called (but then with slot_handle = Slot::unassigned). The
 * callback should return `true` to continue the foreach loop, or return `false` to stop it.
 *
 * \returns Whether the foreach loop came to a natural end. So returns `false` when the callback
 * returned `false`, and `true` otherwise.
 */
bool foreach_action_slot_use(
    const ID &animated_id,
    FunctionRef<bool(const Action &action, slot_handle_t slot_handle)> callback);

/**
 * Same as foreach_action_slot_use(), except that it reports some pointers so the callback can
 * modify which Action/slot is assigned.
 *
 * \see blender::animrig::generic_assign_action
 * \see blender::animrig::generic_assign_action_slot
 * \see blender::animrig::generic_assign_action_slot_handle
 */
bool foreach_action_slot_use_with_references(
    ID &animated_id,
    FunctionRef<bool(bAction *&action_ptr_ref, slot_handle_t &slot_handle_ref, char *slot_name)>
        callback);

}  // namespace blender::animrig
