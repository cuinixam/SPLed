Software Detailed Design
========================

This module interfaces with the keyboard and detects specific key presses, primarily focusing on the POWER_BUTTON_KEY.

.. _module-header:

Module Header
-------------

.. code-block:: c

   #include "power_button.h"
   #include "rte.h"

.. _function-description:

Function Description
--------------------

.. c:function:: void powerButton(void)

.. spec:: Monitor POWER_BUTTON_KEY
   :id: SWDD_KI-001
   :integrity: B

    The function should be called periodically to detect state changes of the POWER_BUTTON_KEY.

.. spec:: Key Pressed Event
   :id: SWDD_KI-002
   :integrity: C

    If the POWER_BUTTON_KEY was just pressed, RteSetPowerKeyPressed will be called with TRUE.

.. spec:: Key Released Event
   :id: SWDD_KI-003
   :integrity: C

    If the POWER_BUTTON_KEY was just released or remains pressed, RteSetPowerKeyPressed will be called with FALSE.


Function Flow
-------------

.. mermaid::

   graph TD
      Start[Start]
      CheckKey[Check if POWER_BUTTON_KEY is pressed]
      WasKeyPressed{Was key pressed in previous state?}
      KeyPressEvent[Send Key Pressed Event]
      KeyReleaseEvent[Send Key Released Event]
      End[End]

      Start --> CheckKey
      CheckKey --> WasKeyPressed
      WasKeyPressed -->|Yes| KeyReleaseEvent --> End
      WasKeyPressed -->|No| KeyPressEvent --> End
