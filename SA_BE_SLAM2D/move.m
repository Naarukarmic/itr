function [ro, RO_r, RO_n] = move(r, u, n)
%   MOVE Robot motion, with separated control and perturbation inputs.
%
%   In:
%       r: robot pose       r = [x ; y ; alpha]
%       u: control signal   u = [d_x ; d_alpha]
%       n: perturbation, additive to control signal
%   Out:
%       ro: updated robot pose
%       RO_r: Jacobian   d(ro) / d(r)
%       RO_n: Jacobian   d(ro) / d(n)

a = r(3);

if nargin > 2
    dx = u(1) + n(1);
    da = u(2) + n(2);
else
    dx = u(1);
    da = u(2);
end
    
ao = a + da;

if ao > pi
    ao = ao - 2*pi;
end
if ao < -pi
    ao = ao + 2*pi;
end

% build position increment dp=[dx;dy], from control signal dx
dp = [dx;0];

if nargout == 1 % No Jacobians requested
    
    to = fromFrame(r, dp);
    
else %  Jacobians requested
    
    [to, TO_r, TO_dt] = fromFrame(r, dp);
    AO_a = 1;
    AO_da = 1;
    
    RO_r = [TO_r ; 0 0 AO_a];
    RO_n = [TO_dt(:,1) zeros(2,1) ; 0 AO_da];
    
end

ro = [to;ao];
