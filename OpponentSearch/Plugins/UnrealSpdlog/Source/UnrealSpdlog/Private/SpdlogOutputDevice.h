#pragma once

/**
 * @brief Outputs all UE_LOG calls to spd structured logging.
 */
class FSpdlogOutputDevice : public FOutputDevice
{
	
public:
	
	/**
	 * @brief Constructor.
	 */
	FSpdlogOutputDevice();

	/**
	 * @brief Create the default SPD logger initialized in the constructor.
	 */
	void MakeSpdDefault() const;

	// Begin FOutputDevice interface
	virtual void Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const FName& Category) override;
	virtual void Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const FName& Category, const double Time) override;
	virtual void Flush() override;
	virtual bool CanBeUsedOnAnyThread() const override;
	virtual bool CanBeUsedOnPanicThread() const override;
	// End FOutputDevice interface
	
private:
	
	static const FString PATTERN;
	
	TUniquePtr<struct FSpdlogLogger> Logger;

};
