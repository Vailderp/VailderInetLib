#pragma once
#include "vildefs.h"
_VIL_BEGIN
class States
{
public:
	virtual ~States() = default;
	
	bool stopped;

	bool muted;

	bool paused;

	States
	(
		const bool stopped = false, 
		const bool muted = false, 
		const bool paused = false
	);
};
_VIL_END

///////////////////////////////////////////////////////////////////////////////

_VIL_BEGIN
class StatesDynamic : protected _VIL States
{
public:
	StatesDynamic(const States states = {});

	void stop(const bool stopped);

	void mute(const bool muted);

	void pause(const bool paused);

	bool stop() const;

	bool mute() const;

	bool pause() const;
};
_VIL_END