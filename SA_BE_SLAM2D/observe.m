function [y, Y_r, Y_p] = observe(r, p)
%   OBSERVE Convert point to robot frame and do a range-and-bearing
%   measurement
%
%   In:
%       r :     robot frame     r = [r_x ; r_y ; r_alpha]
%       p :     point in global frame
%
%   Out: 
%       y :     measurement     y = [range ; bearing]
%       Y_r:    Jacobian wrt r
%       Y_p:    Jacobian wrt p
%
%   (c) 2010, 2011, 2012 Joan Sola

if nargout == 1 % No Jakobian requested
    y = scan(toFrame(r, p));
else % Jocobian requested
    [pr, PR_r, PR_p] = toFrame(r, p);
    [y, Y_pr] = scan(pr);
    % Chain rule
    Y_r = Y_pr * PR_r;
    Y_p = Y_pr * PR_p;
end % if nargout == 1
end