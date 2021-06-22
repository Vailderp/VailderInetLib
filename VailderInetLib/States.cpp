#include "pch.h"
#include "States.h"

_VIL States::States(const bool stopped,
	const bool muted, const bool paused) :
	stopped(stopped),
	muted(muted),
	paused(paused)
{
	
}

vil::StatesDynamic::StatesDynamic(const States states):
	_VIL States(states)
{
	
}

void vil::StatesDynamic::stop(const bool stopped)
{
	this->stopped = stopped;
}

void vil::StatesDynamic::mute(const bool muted)
{
	this->muted = muted;
}

void vil::StatesDynamic::pause(const bool paused)
{
	this->paused = paused;
}

bool vil::StatesDynamic::stop() const
{
	return this->stopped;
}

bool vil::StatesDynamic::mute() const
{
	return this->muted;
}

bool vil::StatesDynamic::pause() const
{
	return this->paused;
}
