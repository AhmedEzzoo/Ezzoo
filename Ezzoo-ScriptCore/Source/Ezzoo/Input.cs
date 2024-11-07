
namespace Ezzoo
{
    public class Input
    {

        public static bool IsKeyDown(KeyCode keyCode)
        {
            return InternalCalls.Entity_IsKeyDown(keyCode);
        }

    }
}
