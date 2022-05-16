#pragma once
// file for misc func declare

class AnimUtils
{
public:
	template<typename T>
	static T* FindNotifyByClass(UAnimSequenceBase* Animation) // UAnimSequenceBase is holder of notifies 
	{
		if (!Animation) return nullptr;
		const auto NotifyEvents = Animation->Notifies;
		// Array of Notifies of montage
		for (auto NotifyEvent : NotifyEvents)
		{
			auto AnimNotify = Cast<T>(NotifyEvent.Notify);
			// Create pointer for notify
			if (AnimNotify) // If notify exist
			{
				return AnimNotify;
			}
		}
		return nullptr;
	}
};