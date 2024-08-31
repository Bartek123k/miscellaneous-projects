using System;
using System.Globalization;
using System.Threading;
using System.Timers;

class Zegar
{
    private static DateTime alarmTime;
    private static bool alarmSet = false;
    private static bool timerSet = false;
    private static TimeSpan timerDuration;
    private static DateTime timerEndTime;
    private static readonly object consoleLock = new object();
    private static bool alarmTriggered = false;
    private static bool timerEnded = false;
    private static int commandInputTop = 10;
    private static int outputStartLine = 12;
    private static bool displayTimerInsteadOfTime = false;
    static void Main(string[] args)
    {
        Thread clockThread = new Thread(new ThreadStart(StartClock));
        clockThread.IsBackground = true;
        clockThread.Start();
        DisplayCommands();
        while (true)
        {
            lock (consoleLock)
            {
                Console.SetCursorPosition(0, commandInputTop);
                Console.Write(new string(' ', Console.WindowWidth));
                Console.SetCursorPosition(0, commandInputTop);
            }
            string? input = Console.ReadLine();
            if (input == null) continue;
            string[] commandArgs = input.Split(' ');
            lock (consoleLock)
            {
                Console.SetCursorPosition(0, outputStartLine);
                for (int i = outputStartLine; i < Console.WindowHeight; i++)
                {
                    Console.SetCursorPosition(0, i);
                    Console.Write(new string(' ', Console.WindowWidth));
                }
                Console.SetCursorPosition(0, outputStartLine);
                if (commandArgs.Length > 1)
                {
                    switch (commandArgs[0])
                    {
                        case "-alarm":
                            if (DateTime.TryParseExact(commandArgs[1], "HH:mm", CultureInfo.InvariantCulture, DateTimeStyles.None, out alarmTime))
                            {
                                alarmSet = true;
                                alarmTriggered = false;
                                Console.WriteLine($"Alarm ustawiony na {alarmTime:HH:mm}");
                            }
                            else
                            {
                                Console.WriteLine("Niepoprawny format czasu dla alarmu. Użyj HH:mm.");
                            }
                            break;
                        case "-timer":
                            if (TimeSpan.TryParseExact(commandArgs[1], "m\\:ss", CultureInfo.InvariantCulture, out timerDuration))
                            {
                                timerSet = true;
                                timerEnded = false;
                                timerEndTime = DateTime.Now.Add(timerDuration);
                                Console.WriteLine($"Timer ustawiony na {timerDuration:mm\\:ss}");
                                displayTimerInsteadOfTime = true;
                            }
                            else
                            {
                                Console.WriteLine("Niepoprawny format czasu dla timera. Użyj m:ss.");
                            }
                            break;
                        default:
                            Console.WriteLine("Niepoprawny argument. Użyj -alarm lub -timer.");
                            break;
                    }
                }
                else
                {
                    Console.WriteLine("Niepoprawna liczba argumentów.");
                }
            }
        }
    }
    private static void StartClock()
    {
        System.Timers.Timer timer = new System.Timers.Timer(1000);
        timer.Elapsed += OnTimedEvent;
        timer.AutoReset = true;
        timer.Enabled = true;
    }
    private static void OnTimedEvent(object source, ElapsedEventArgs e)
    {
        lock (consoleLock)
        {
            int cursorLeft = Console.CursorLeft;
            int cursorTop = Console.CursorTop;
            Console.SetCursorPosition(0, 4);
            for (int i = 0; i < 3; i++)
            {
                Console.Write(new string(' ', Console.WindowWidth));
            }
            Console.SetCursorPosition(0, 4);
            if (displayTimerInsteadOfTime)
            {
                DisplayTimer(timerDuration);
            }
            else
            {
                DisplayTime(DateTime.Now);
            }
            if (alarmSet)
            {
                CheckAlarm();
            }
            if (timerSet)
            {
                timerDuration = timerEndTime - DateTime.Now;
                if (timerDuration.TotalSeconds <= 0 && !timerEnded)
                {
                    timerEnded = true;
                    timerSet = false;
                    displayTimerInsteadOfTime = false;
                    Console.SetCursorPosition(0, outputStartLine + 1);
                    Console.WriteLine("Koniec czasu!");
                }
            }
            Console.SetCursorPosition(cursorLeft, cursorTop);
        }
    }
    private static void CheckAlarm()
    {
        DateTime now = DateTime.Now;
        DateTime alarm = new DateTime(now.Year, now.Month, now.Day, alarmTime.Hour, alarmTime.Minute, 0);
        if (now >= alarm && !alarmTriggered)
        {
            alarmTriggered = true;
            Console.SetCursorPosition(0, outputStartLine + 1);
            Console.ForegroundColor = ConsoleColor.Red;
            for (int i = 0; i < 5; i++)
            {
                Console.WriteLine("ALARM!");
                Console.Beep();
                Thread.Sleep(500);
            }
            Console.ResetColor();
        }
    }
    private static void DisplayCommands()
    {
        Console.Clear();
        Console.WriteLine("Aby ustawić timer lub alarm, użyj odpowiednich komend:");
        Console.WriteLine("-alarm HH:mm");
        Console.WriteLine("-timer mm:ss");
        Console.WriteLine();
        Console.SetCursorPosition(0, commandInputTop);
    }
    private static void DisplayTime(DateTime time)
    {
        Console.SetCursorPosition(0, 4);
        Console.WriteLine("Aktualny Czas:");
        DisplaySegment(time.Hour.ToString("D2") + ":" + time.Minute.ToString("D2"));
    }
    private static void DisplayTimer(TimeSpan timeSpan)
    {
        Console.SetCursorPosition(0, 4);
        Console.WriteLine("Timer:");
        DisplaySegment(timeSpan.Minutes.ToString("D2") + ":" + timeSpan.Seconds.ToString("D2"));
    }
    private static void DisplaySegment(string input)
    {
        string[] segments =
        {
            " _     _  _     _  _  _  _  _ ",
            "| |  | _| _||_||_ |_   ||_||_|",
            "|_|  ||_  _|  | _||_|  ||_| _|"
        };
        int segmentHeight = segments.Length;
        Console.ForegroundColor = ConsoleColor.Green;
        for (int i = 0; i < segmentHeight; i++)
        {
            foreach (char c in input)
            {
                if (char.IsDigit(c))
                {
                    int digit = c - '0';
                    Console.Write(segments[i].Substring(digit * 3, 3));
                }
                else if (c == ':')
                {
                    if (i == 1 || i == 2)
                    {
                        Console.Write(" . ");
                    }
                    else
                    {
                        Console.Write("   ");
                    }
                }
            }
            Console.WriteLine();
        }
        Console.ResetColor();
    }
}